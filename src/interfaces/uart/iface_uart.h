/**
 * @file    iface_uart.h
 * @author  Jose Miguel Rios Rubio <jrios.github@gmail.com>
 * @date    2024-01-04
 * @version 1.0.0
 *
 * @section DESCRIPTION
 *
 * ESPMULTILOG Universal Asynchronous Receiver-Transmitter (UART) Interface
 * header file.
 *
 * @section LICENSE
 *
 * MIT License
 *
 * Copyright (c) 2024 Jose Miguel Rios Rubio
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

/*****************************************************************************/

/* Include Guard */

#ifndef INTERFACE_UART_H
#define INTERFACE_UART_H

/*****************************************************************************/

/* Libraries */

/*****************************************************************************/

/* Class Interface */

class InterfaceUART
{
    /******************************************************************/

    /* Private Constants */

    private:

    /******************************************************************/

    /* Public Constants */

    public:

    /******************************************************************/

    /* Private Data Types */

    private:

    /******************************************************************/

    /* Public Data Types */

    public:

    /******************************************************************/

    /* Public Methods */

    public:

        /**
         * @brief Construct a new Interface object.
         */
        InterfaceUART();

        /**
         * @brief Initializes the Interface.
         */
        void init();

        /**
         * @brief Manage the Interface.
         */
        void process();

    /******************************************************************/

    /* Private Methods */

    private:

    /******************************************************************/

    /* Public Attributes */

    public:

    /******************************************************************/

    /* Private Attributes */

    private:

    /******************************************************************/
};

/*****************************************************************************/

/* Object Declaration */

/**
 * @brief Interface Declaration.
 */
extern InterfaceUART IfaceUART;

/*****************************************************************************/

/* Include Guard Close */

#endif /* INTERFACE_UART_H */
