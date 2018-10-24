#ifndef GLAS_MATERIAL_H
#define GLAS_MATERIAL_H

#include "materials/material.h"
#include "math/mathutil.h"
#include "math/vec.h"
#include "core/random.h"

class GlasMaterial : public Material
{
    private:
    Color color;

    public:
    GlasMaterial(Color col) : color(col) {}

    void sample(Query &query)
    {
        query.color = color;

        vec n = query.normal;
        vec nl = (n * query.dir) < 0 ? n : n*real(-1);
        vec reflDir = query.dir - n * (real(2) * (n * query.dir));

        bool into = (n * nl) > real(0);                // Ray from outside going in?
        
        real nc(1);
        real nt(1.5);

        real nnt = into ? (nc/nt) : (nt/nc);
        real ddn = query.dir * nl;
        real cos2t;
        if((cos2t = real(1) - nnt * nnt * (real(1) - ddn * ddn)) < real(0)){    // Total internal reflection
            query.outdir = reflDir;
            query.type = REFLECTIVE;
            return;
        }
        vec tdir = ((query.dir * nnt) - n * ( (into ? real(1) : real(-1) ) * ( (ddn * nnt) + Math::sqrt(cos2t) )));
        tdir.normalize();
        real a = nt-nc;
        real b = nt+nc;
        real R0 = a*a/(b*b);
        real c = real(1) - (into ? (-ddn) : (tdir * n));
        real Re = R0 + (real(1)-R0) *c*c*c*c*c;
        real Tr = real(1) - Re;
        real P = real(0.25) + real(0.5) * Re;
        real RP = Re/P;
        real TP = Tr/(real(1)-P);

        if (Random::getSample1D()<P){
            query.color.scale(RP);
          //cf.scale(RP);
            query.outdir = reflDir;
            query.type = REFLECTIVE;
        } else {
          //cf.scale(TP);
            query.color.scale(TP);

            if(!into)
            {
                //beers law
                //real factor(0.20);
                //query.color.scale(Math::exp(-factor * query.distance));
            }

            query.outdir = tdir;
            query.type = TRANSMISSIVE;
        }
    }

    Color getColor()
    {
        return color;
    }
};

#endif
