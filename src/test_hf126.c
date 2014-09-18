/*
 * Copyright (C) 2014 Mentor Graphics
 * This file is a test application.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <systemd/sd-daemon.h>

int main(void)
{
    pid_t pid = fork();
    switch (pid)
    {
        case 0: // Child process
            // close everything
            umask(0);
            chdir("/");
            close(STDIN_FILENO);
            close(STDOUT_FILENO);
            close(STDERR_FILENO);
            setsid();

            // pause
            sleep(1);

            // fork one more time
            pid = fork();
            if (pid == -1)
            {
                exit(1);
            }
            else if (pid > 0)
            {
                exit(0);
            }

            // notify systemd that the service start up is complete
            sd_notify(0, "READY=1");

            // main loop of the daemon
            while (1)
            {
                sleep(1);
            }

            break;

        case -1:
            fprintf(stderr, "ERROR: fork failed\n");
            exit(1);

        default: // Parent process
            // set main pid to 0
            sd_notifyf(0, "MAINPID=%lu", (unsigned long)pid);

            // stay alive for a while
            sleep(10);
            exit(0);
    }
}
