

#include <stdio.h>
#include <errno.h>
#include <argp.h>
#include "dictionary.h"
//#include "spell.h"


const char *argp_program_version = "spell_checker 1.0";

struct arguments
{
    char *args[1];          // Arguments 1 and 2
    int verbose;            // -v verbosity flag
    char *checked_file;     // File to be compared against
    char *output_file;      // Optionally set file for output
};



static struct argp_option options[] =
        {
                {"verbose", 'v', 0, 0, "Produce verbose output"},
                {"checked-file", 'c', "INPUT", 0, "Set checked file"},
                {"output_file", 'o', "OUTPUT", 0, "Optionally set output file "},
                {0}
        };
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
    struct arguments *arguments = state->input;

    switch(key) {
        case 'v':
            arguments->verbose = 1;
            break;
        case 'c':
            arguments->checked_file = arg;
            break;
        case 'o':
            arguments->output_file = arg;
            break;
        case ARGP_KEY_ARG:
            if (state->arg_num >= 1) {
                argp_usage(state);
            }
            arguments->args[state->arg_num] = arg;
            break;
        case ARGP_KEY_END:
            if (state->arg_num < 1) {
                argp_usage(state);
            }
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}


// Documentation for required argument
static char args_doc[] = "DICT_FILE";

// Documentation purpose of program
static char doc[] = "A spell checker";

// Documentation optional flags
static struct argp argp = {options, parse_opt, args_doc, doc};


#ifndef DOING_UNIT_TESTS
int main(int argc, const char ** argv) {

    struct arguments arguments;
    FILE *outstream;
    FILE *input;
    char * misspelled[MAX_MISSPELLED] = { 0 };
    hashmap_t hashtable[HASH_SIZE] = { 0 };

    arguments.output_file = NULL;
    arguments.checked_file = NULL;
    arguments.verbose = 0;
    argp_parse(&argp, argc, (char **) argv, 0, 0, &arguments);

    if (arguments.output_file)
        outstream = fopen(arguments.output_file, (const char *) "w");
    else
        outstream = stdout;
    if (arguments.checked_file)
        input = fopen(arguments.checked_file, "r");
    else
        input = NULL;

    if (arguments.verbose) {
        printf("VERBOSE");
    }

    load_dictionary(arguments.args[0], hashtable);
    int miss_num = check_words(input, hashtable, misspelled);
    printf("%d\n", miss_num);
    return 0;
}

#endif
