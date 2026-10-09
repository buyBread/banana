#include "treyarch/chuck/vm/script_executable.hh"

using namespace treyarch;
using namespace treyarch::chuck::vm;

// sub_A1F660
void script_executable::register_callbacks(resolve_signal_callback_t               resolve_signal,
                                           resolve_extern_callback_t               resolve_extern,
                                           get_chuck_client_library_key_callback_t get_chuck_client_library_key,
                                           get_script_executable_folder_callback_t get_script_executable_folder) {

    references::resolve_signal_callback              .write(resolve_signal);
    references::resolve_extern_callback              .write(resolve_extern);
    references::get_chuck_client_library_key_callback.write(get_chuck_client_library_key);
    references::get_script_executable_folder_callback.write(get_script_executable_folder);
}
