#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum TileSet {
    WALL  = 'X',
    SPACE = '.',
    START = 'S',
    END   = 'E'
};

struct Args {
    unsigned int width;
    unsigned int height;

    enum Algo { RANDOM = 0, DEPTHFIRST } algo;
};

int ppArgs(const struct Args *args, FILE *out) {
    if (args == NULL || out == NULL) {
        return 0;
    }

    const char *algoName = NULL;
    switch (args->algo) {
    case RANDOM:        algoName = "Random";        break;
    case DEPTHFIRST:    algoName = "Depth First";   break;
    default:            algoName = "<Unknown>";     break;
    }

    return fprintf(out, "width: %u, height: %u algo: '%s'", args->width, args->height, algoName);
}

int randInRange(const int low, const int high) {
    if (low > high) {
        assert(0 && "low must be less then or equal to high");
    } 

    if (low == high) {
        return low;
    }

    /* introduces a slight bias, should be good enough for now */
    return (rand() % (high - low)) + low;
}

int randomMaze(FILE *out, const unsigned int width, const unsigned int height) {
    char *maze = calloc(width, height);

    srand(0); /* seed with 0 for repeatable results */   

    for (char *it = maze, *end = maze + width * height; it < end; it++) {
        *it = randInRange(0, 4) < 1 ? WALL : SPACE;
    }

    /* put the start and end in the corners */
    maze[0] = START;
    maze[width * height - 1] = END;      

    fwrite(maze, width, height, out);

    return 0; 
}

int dfsMaze(FILE *out, const unsigned int width, const unsigned int height) {

    return 0;
}

int createMaze(FILE *out, const struct Args *args) {
    if (out == NULL || args == NULL) {
        return 0;
    }
 
    fprintf(stderr, "Creating maze with ");
    ppArgs(args, stderr);
    fprintf(stderr, "\n");

    const unsigned int width  = args->width;
    const unsigned int height = args->height; 

    switch (args->algo) {
    case RANDOM:        return randomMaze(out, width, height);
    case DEPTHFIRST:    return dfsMaze(out, width, height);
    }

    return -1;
}

int main(int argc, char ** argv) {
    FILE *output = stdout;
    struct Args args = { 0, 0, RANDOM };

    if (argc < 3) {
        fputs("Error: Not enough arguments specified - exiting\n", stderr);
        return -1;
    }

    args.width  = atoi(argv[1]);
    args.height = atoi(argv[2]);

    if (args.width * args.height <= 1) {
        fputs("Error: Maze area must be greater then 1\n", stderr);
        return -1;
    }

    return createMaze(output, &args);
}
