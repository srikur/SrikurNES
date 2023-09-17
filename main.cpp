#include "core/common.h"
#include "nes.h"

using std::vector;
using std::ifstream;
using std::cout;

int main (int argc, char *argv[]) {
  /* If less than one argument provided, exit */
  if (argc < 2) {
    cout << "Usage: " << argv[0] << " <rom file>\n";
    return 1;
  }

  /* Open ROM file and check header */
  ifstream romFile(argv[1], std::ios::binary);
  if (!romFile) {
    cout << "Error: Could not open file\n";
    return 1;
  }

  std::unique_ptr<NES> nes = std::make_unique<NES>();

  // Read ROM into memory
  vector<u8> romData((std::istreambuf_iterator<char>(romFile)), {});

  checkHeader(romData);

  /* Main event loop */
  bool quit = false;
  while (!quit) {
  }

  return 0;
}

