
/* Include Guard */

#ifndef COMMANDS_H
#define COMMANDS_H

/*****************************************************************************/

/* Class Interface */

class CommandLineInterface
{
    /******************************************************************/

    /* Public Methods */

    public:

        /**
         * @brief Initializes the Command Line Interface.
         */
        void init();

        /**
         * @brief Manage the Command Line Interface.
         */
        void process();

    /******************************************************************/
};

/*****************************************************************************/

/* Object Declaration */

/**
 * @brief Command Line Interface Declaration.
 */
extern CommandLineInterface CLI;

/*****************************************************************************/

/* Include Guard Close */

#endif /* COMMANDS_H */
