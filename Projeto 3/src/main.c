#include <stdio.h>

#include "disjointset.h"
#include "svg.h"
#include "city.h"
#include "environment.h"
#include "resolver.h"

int main(int argc, char** argv){

    Env myEnv = setEnv(argc, argv);

    City bitnopolis = newCity();

    Svg svg = newSvg(), 
        qrySvg = NULL, 
        querries = NULL;

    

    solveGeo(bitnopolis, myEnv);

    solvePm(bitnopolis, myEnv);

    solveSvg(bitnopolis, svg, NULL);

    if(isQueryActive(myEnv)){

        qrySvg = newSvg();
        querries = newSvg();

        solveQry(bitnopolis, querries, myEnv);
    }

    solveSvg(bitnopolis, qrySvg, querries);

    writeSvg(svg, getSvgPath(myEnv));

    writeSvg(qrySvg, getQrySvgPath(myEnv));

    freeSvg(svg);

    freeSvg(qrySvg);

    freeSvg(querries);

    destroyCity(bitnopolis);

    freeEnv(myEnv);

    return 0;
}