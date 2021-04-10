#include "dashbcpp.h"

using namespace std;
using std::string;

string getDate() {
  time_t rawDate;
  struct tm *dateInfo;
  char buffer[90];

  time(&rawDate);
  dateInfo = localtime(&rawDate);
  strftime(buffer, 90, "%F %A", dateInfo);
  string str(buffer);
  return str;
}

string getTime() {
  time_t rawtime;
  struct tm *timeInfo;
  char buffer[80];

  time(&rawtime);
  timeInfo = localtime(&rawtime);
  strftime(buffer, 80, "%X", timeInfo);
  string str(buffer);
  return str;
}

void keyEvent() {
  int c = getch();
  switch (c) {
  case 'q':
  case 'Q':
    dashbcpp.running = false;
    break;
  default:
    refresh();
  }
  return;
}

int64_t getTotalRam() {

  int mib[2];
  int64_t physical_memory;
  mib[0] = CTL_HW;
  mib[1] = HW_MEMSIZE;
  size_t length = sizeof(int64_t);
  sysctl(mib, 2, &physical_memory, &length, NULL, 0);

  return physical_memory / 1024 / 1024 / 1024;
}

long long *getRam() {
  vm_size_t page_size;
  mach_port_t mach_port;
  mach_msg_type_number_t count;
  vm_statistics64_data_t vm_stats;

  mach_port = mach_host_self();
  count = sizeof(vm_stats) / sizeof(natural_t);
  if (KERN_SUCCESS == host_page_size(mach_port, &page_size) &&
      KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                                        (host_info64_t)&vm_stats, &count)) {
    long long free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size;

    long long used_memory =
        ((int64_t)vm_stats.active_count + (int64_t)vm_stats.inactive_count +
         (int64_t)vm_stats.wire_count) *
        (int64_t)page_size;
    long long *array = new long long[2];
    array[0] = free_memory / 1024 / 1024;
    array[1] = used_memory / 1024 / 1024;
    ;
    return array;
  }
  return 0;
}

long long getFreeSpace() {
  struct statfs vfs;
  if (statfs(".", &vfs) == -1) {
    return 0;
  } else {
    return (vfs.f_bsize * vfs.f_bfree) / 1024 / 1024 / 1024;
  }
}
long long getTotalSpace() {
  struct statfs vfs;
  if (statfs(".", &vfs) == -1) {
    return 0;
  } else {
    return (vfs.f_bsize * vfs.f_blocks) / 1024 / 1024 / 1024;
  }
}

int main() {
  initscr();
  /* cbreak(); */
  timeout(1000);
  noecho();
  curs_set(0);

  SMCOpen();

  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);

  /* WINDOW *win = newwin(yMax / 2, xMax / 2, yMax / 4, xMax / 4); */
  /* box(win, 0, 0); */
  dashbcpp.running = true;
  while (dashbcpp.running) {
    keyEvent();
    /* struct timespec delay = { */
    /*     0,        /1* seconds *1/ */
    /*     500000000 /1* nanoseconds *1/ */
    /* }; */

    /* fd_set rfds; */
    /* FD_ZERO(&rfds); */
    /* FD_SET(STDIN_FILENO, &rfds); */

    /* nanosleep(&delay, NULL); */
    /* pselect(1, &rfds, NULL, NULL, &delay, NULL); */

    clear();
    mvprintw(0, xMax - 9, getTime().c_str());
    mvprintw(0, 1, getDate().c_str());
    mvprintw(yMax - 3, 1, "CPU ");
    mvprintw(yMax - 2, 1, "%.f%%", GetCPULoad());
    mvprintw(yMax - 1, 1, "%.1fºC", SMCGetTemperature((char *)"TC0P"));

    long long *ram = getRam();
    mvprintw(yMax - 3, 10, "RAM (%lldGb)", getTotalRam());
    mvprintw(yMax - 2, 10, "Free %.1f Gb", ram[0] / 1024.f);
    mvprintw(yMax - 1, 11, "Used %.1f Gb", ram[1] / 1024.f);

    mvprintw(yMax - 3, 25, "Disk");
    mvprintw(yMax - 2, 25, "Free: %llu/%llu Gb", getFreeSpace(),
             getTotalSpace());

    refresh();
  }

  SMCClose();
  endwin();
  return 0;
}
