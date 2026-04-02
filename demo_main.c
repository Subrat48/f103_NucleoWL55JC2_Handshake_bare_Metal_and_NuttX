/****************************************************************************
 * apps/examples/demo/demo_main.c
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

#define UART_1 "/dev/ttyS1"
#define HS_CMD_LEN 4
#define DELAY_IN_US 100000

/****************************************************************************
 * Private Data
 ****************************************************************************/

static const uint8_t g_hs_cmd[HS_CMD_LEN] =
{
  0xaa, 0xbb, 0xcc, 0xdd
};

static volatile int g_handshake_flag = 0;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: handshake_stm32f103
 ****************************************************************************/

static int handshake_stm32f103(void)
{
  int fd_hs;
  int wr_hs;
  int rd_hs;
  int i;
  uint8_t hs_cmd_temp[HS_CMD_LEN] =
  {
    '\0'
  };

  printf("Sending Handshake command to port %s\r\n", UART_1);

  fd_hs = open(UART_1, O_RDWR);
  if (fd_hs < 0)
    {
      printf("??? unable to open the port %s\r\n", UART_1);
      usleep(DELAY_IN_US);
      g_handshake_flag = 0;
      return g_handshake_flag;
    }

  wr_hs = write(fd_hs, g_hs_cmd, HS_CMD_LEN);
  if (wr_hs < 0)
    {
      printf("??? unable to write to port %s\r\n", UART_1);
      usleep(DELAY_IN_US);
      close(fd_hs);
      g_handshake_flag = 0;
      return g_handshake_flag;
    }

  printf("--> %d bytes successfully written in port %s\r\n", wr_hs, UART_1);

  for (i = 0; i < wr_hs; i++)
    {
      printf(" %02x ", g_hs_cmd[i]);
    }

  printf("\r\n");

  tcdrain(fd_hs);
  ioctl(fd_hs, TCFLSH, 2);

  printf("--> tx rx buffer flushed\r\n");
  usleep(DELAY_IN_US);

  printf("Wait to receive handshake command via port %s\r\n", UART_1);

#if 0
  /* --- ORIGINAL CODE --- */

  for (i = 0; i < HS_CMD_LEN; i++)
    {
      rd_hs = read(fd_hs, &hs_cmd_temp[i], 1);
    }
#endif

  for (i = 0; i < HS_CMD_LEN; i++)
    {
      rd_hs = read(fd_hs, &hs_cmd_temp[i], 1);

      if (rd_hs <= 0)
        {
          printf("??? Read timeout or error on port %s\r\n", UART_1);
          break;
        }
    }

  printf("--> Handshake command received from port %s\r\n", UART_1);

  for (i = 0; i < HS_CMD_LEN; i++)
    {
      printf(" %02x ", hs_cmd_temp[i]);
    }

  printf("\r\n");

  usleep(DELAY_IN_US);

  if (memcmp(g_hs_cmd, hs_cmd_temp, HS_CMD_LEN) == 0)
    {
      printf("--> Handshake operation successful\r\n");
      g_handshake_flag = 1;
    }
  else
    {
      printf("??? Handshake operation failed\r\n");
      g_handshake_flag = 0;
    }

  usleep(DELAY_IN_US);
  ioctl(fd_hs, TCFLSH, 2);
  close(fd_hs);

  return g_handshake_flag;
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: main
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  printf("Starting Bluepill Handshake Sequence...\r\n");

  while (g_handshake_flag != 1)
    {
      handshake_stm32f103();
      usleep(500000);
    }

  printf("\r\nOperation Starts!\r\n");

  return 0;
}
