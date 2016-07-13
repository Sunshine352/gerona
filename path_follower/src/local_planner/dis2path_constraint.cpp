/// HEADER
#include <path_follower/local_planner/dis2path_constraint.h>

Dis2Path_Constraint::Dis2Path_Constraint():
    Constraint(),limit(0.3)
{

}

Dis2Path_Constraint::~Dis2Path_Constraint()
{

}

void Dis2Path_Constraint::setLimit(double new_limit){
    if(new_limit > limit){
        limit = new_limit;
    }
}

bool Dis2Path_Constraint::isSatisfied(const LNode& point){
    sw.resume();
    if(point.d2p <= limit){
        limit *= limit > 0.3?0.95:1.0;
        limit = limit < 0.3?0.3:limit;
        sw.stop();
        return true;
    }else{//If an obstacle decrease the space around the path, then the space boundary is extended.
        if(point.d2o < std::numeric_limits<double>::infinity()){
            double x = point.nop.x - point.x;
            double y = point.nop.y - point.y;
            double orio = atan2(y,x);
            x = point.npp.x - point.x;
            y = point.npp.y - point.y;
            double orip = atan2(y,x);
            double adiff = orio - orip;
            adiff += (adiff > M_PI) ? - 2.0*M_PI : (adiff < -M_PI) ? 2.0*M_PI : 0;
            adiff = abs(adiff);
            if(adiff <= M_PI/6){
                double tol = 1.15 - (limit - 0.3) - (point.d2o - cos(adiff)*point.d2p);
                if(tol > 0.0){
                    limit += tol;
                    if(point.d2p <= limit){
                        sw.stop();
                        return true;
                    }
                }
            }
        }
    }
    sw.stop();
    return false;
}
