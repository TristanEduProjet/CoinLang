#include "unorderedmap.h"

/*
 * Hack pour permettre l'initialisation de la variable globale
 */

extern "C" {
    const HashMap *variables = HashMap_new();
}
