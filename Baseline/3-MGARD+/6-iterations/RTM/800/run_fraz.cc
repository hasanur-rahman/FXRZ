#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <libpressio.h>
#include <mpi.h>
#include <libpressio_ext/io/pressio_io.h>
#include <vector>
#include <map>
#include <string>
// helper functions {{{
#include <libpressio_ext/cpp/dtype.h>
template <class T>
void helper_set_data(pressio_options* opts, const char* key, std::initializer_list<T> const& il) {
  std::vector<T> arr(il.begin(), il.end());
  size_t size = arr.size();
  pressio_data* data = pressio_data_new_copy(pressio_dtype_from_type<T>(), arr.data(), 1, &size);
  pressio_options_set_data(opts, key, data);
  pressio_data_free(data);
}
// }}}

int main(int argc, char *argv[]) {
  int provided, rank;
  const char *datapath = argv[1];
  const double target_cratio = 1.0 * atof(argv[2]); 
  printf("target cratio %.2lf begin\n", target_cratio);
  //MPI_Init(&argc,&argv)
  MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (provided != MPI_THREAD_MULTIPLE) {
    fprintf(
        stderr,
        "MPI Thread Multiple not provided, check your MPI implementation\n");
    MPI_Finalize();
    exit(1);
  }

  pressio *library = pressio_instance();
  pressio_compressor *compressor = pressio_get_compressor(library, "opt");
  if (!compressor) {
    fprintf(stderr, "%s\n", pressio_error_msg(library));
    MPI_Finalize();
    exit(pressio_error_code(library));
  }

  pressio_options *options = pressio_options_new();
  
  //
  pressio_options_set_string(options, "opt:compressor", "pressio");
  pressio_options_set_string(options, "pressio:compressor", "mgardx");

  pressio_options_set_string(options, "opt:metric", "composite");
  pressio_options_set_string(options, "opt:search", "dist_gridsearch");
  pressio_options_set_string(options, "dist_gridsearch:search", "fraz");
  pressio_options_set_string(options, "opt:search_metrics", "progress_printer");
  pressio_options_set_string(options, "mgardx:metric", "composite");
  pressio_options_set_string(options, "opt:objective_mode_name", "target");
  pressio_options_set_uinteger(options, "opt:do_decompress", 0);
  const char *metric_ids[] = {"size", "time"};

  helper_set_data(options, "dist_gridsearch:overlap_percentage", {0.2});
  pressio_options_set_double(options, "opt:global_rel_tolerance", 0.2);
  pressio_options_set_strings(options, "composite:plugins",
                              std::size(metric_ids), metric_ids);

 
  pressio_options_set_string(options, "time:metric", "error_stat");
  const char* inputs[] = { 
    "pressio:rel"
  };
  const char* outputs[] = {
    "size:compression_ratio"
  };
  pressio_options_set_strings(options, "opt:inputs", std::size(inputs), inputs);
  pressio_options_set_strings(options, "opt:output", std::size(outputs), outputs);

  pressio_options_set_double(options, "opt:target", target_cratio);
  helper_set_data(options, "dist_gridsearch:num_bins", {3.0});
  helper_set_data(options, "opt:lower_bound", {0.0});
  helper_set_data(options, "opt:upper_bound", {0.0015});
  pressio_options_set_uinteger(options, "opt:max_iterations", 2);
  pressio_options_set_uinteger(options, "fraz:nthreads", 1);
  pressio_options_set_uinteger(options, "opt:inter_iteration", 0); //set to 0 for FRaZ, 1 for FMFS/OptZConfig



  if(pressio_compressor_set_options(compressor, options)) {
  }
  pressio_options_free(options);

  if(rank == 0) {
    options = pressio_compressor_get_options(compressor);
    char *str = pressio_options_to_string(options);
    fprintf(stderr, "%s\n", str);
    free(str);
    pressio_options_free(options);
  }

  pressio_io* io = pressio_get_io(library, "posix");
  pressio_options* io_options = pressio_options_new();
  pressio_options_set_string(io_options, "io:path", datapath);
  pressio_io_set_options(io, io_options);
  size_t dims[] = {849, 849, 235};
  pressio_data* metadata = pressio_data_new_owning(pressio_float_dtype, std::size(dims), dims);
  pressio_data* input = pressio_io_read(io, metadata);
  if(input == nullptr) {
    fprintf(stderr, "%s\n", pressio_io_error_msg(io));
    MPI_Finalize();
    exit(pressio_io_error_code(io));
  }
  pressio_data* output = pressio_data_new_clone(input);
  pressio_data* compressed = pressio_data_new_empty(pressio_byte_dtype, 0, nullptr);

  if(pressio_compressor_compress(compressor, input, compressed)) {
    fprintf(stderr, "%s\n", pressio_compressor_error_msg(compressor));
    MPI_Finalize();
    exit(pressio_compressor_error_code(compressor));
  }
  if(pressio_compressor_decompress(compressor, compressed, output)) {
    fprintf(stderr, "%s\n", pressio_compressor_error_msg(compressor));
    MPI_Finalize();
    exit(pressio_compressor_error_code(compressor));
  }


  if(rank == 0) {
    options = pressio_compressor_get_metrics_results(compressor);
    char* str = pressio_options_to_string(options);
    fprintf(stderr, "%s\n", str);
    printf("<<<<<<<<< target cratio %.2lf end >>>>>>>>\n", target_cratio);
    free(str);
    pressio_options_free(options);
  }

  pressio_data_free(metadata);
  pressio_data_free(input);
  pressio_compressor_release(compressor);
  pressio_release(library);
  MPI_Finalize();
  return 0;
}

// vim: foldmethod=marker :
