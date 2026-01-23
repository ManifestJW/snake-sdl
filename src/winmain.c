#ifdef _WIN32
#  include <stdlib.h>
#  include <windows.h>

extern int main(int argc, char **argv);

int WINAPI WinMain(HINSTANCE instance,
                   HINSTANCE prev_instance,
                   LPSTR cmd_line,
                   int show_cmd) {
  (void)instance;
  (void)prev_instance;
  (void)cmd_line;
  (void)show_cmd;

  return main(__argc, __argv);
}
#endif
