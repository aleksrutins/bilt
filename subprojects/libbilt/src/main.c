/* main.c
 *
 * Copyright 2021 Aleks Rutins
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// https://stackoverflow.com/a/4309845
char *fgets_wrapper(char *buffer, size_t buflen, FILE *fp)
{
    if (fgets(buffer, buflen, fp) != 0)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        return buffer;
    }
    return 0;
}

int bilt_run_cmd(char *cmd) {
  printf("\e[32mrun-cmd\e[0m\t%s\n", cmd);
  return system(cmd);
}
void bilt_info(char *msg) {
  printf("\e[96minfo\e[0m\t%s\n", msg);
}
void bilt_warn(char *msg) {
  printf("\e[93mwarn\e[0m\t%s\n", msg);
}
void bilt_err(char *msg) {
  fprintf(stderr, "\e[1;31merr\e[0m\t%s\n", msg);
}
void bilt_prompt(char *msg, char *buf, size_t length) {
  printf("\e[36mprompt\e[0m\t%s\n", msg);
  printf("\e[36manswer\e[0m\t");
  fgets_wrapper (buf, length, stdin);
}
