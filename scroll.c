#include <linux/input.h>
#include <linux/uinput.h>

int main() {
  struct input_event event;
  int fd;

  // Open the input device file.
  fd = open("/dev/input/event", O_RDONLY);
  if (fd < 0) {
    return -1;
  }

  // Read the scroll event data from the input device file.
  while (read(fd, &event, sizeof(event)) > 0) {
    // Check if the event is a scroll event.
    if (event.type == EV_REL && event.code == REL_WHEEL) {
      // Get the scroll delta.
      int delta = event.value;

      // Do something with the scroll delta.
      // For example, you could print the delta to the console.
      printf("Scroll delta: %d\n", delta);
    }
  }

  // Close the input device file.
  close(fd);

  return 0;
}