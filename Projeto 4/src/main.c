#include <stdio.h>

#include "svg.h"
#include "city.h"
#include "environment.h"
#include "resolver.h"

int main(int argc, char** argv){

    Env myEnv = env_set(argc, argv);

    City bitnopolis = newCity();

    SvgFile svg = newSvgFile(), 
            qrySvg = NULL, 
            queries = NULL;

    solveGeo(bitnopolis, myEnv);

    solveVia(bitnopolis, myEnv);

    solveSvg(bitnopolis, svg, NULL);

    if(env_isQueryActive(myEnv)){

        qrySvg = newSvgFile();
        queries = newSvgFile();

        solveQry(bitnopolis, qrySvg, queries, myEnv);
    }

    solveSvg(bitnopolis, qrySvg, queries);

    svg_write(svg, env_getSvgPath(myEnv));

    svg_write(qrySvg, env_getQrySvgPath(myEnv));

    svg_free(svg);

    svg_free(qrySvg);

    svg_free(queries);

    city_free(bitnopolis);

    env_free(myEnv);

    return 0;
}