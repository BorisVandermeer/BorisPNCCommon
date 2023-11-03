/*********************************************************************
 * Author : BorisVandermeer
 * 
 * Discription ：
 *      A Descripe Vehicle with servel shapes
 * 
*********************************************************************/
#pragma once
#include<cmath>
#include<Models/VehicleModel.h>

namespace Models{
    
    template<int N = 2>
    class VehicleShapeCircles : public VehicleShape{
    public:
        VehicleShapeCircles() = delete;
        VehicleShapeCircles(VehicleShape _shape);
        double Radius;
        std::vector<double> CircleCenters; // Distance to rear axle center;
        std::vector<Points::PosPoint2D> getCircleCenters(const Points::Pos2D & CurPos);
    };

    template<int N>
    VehicleShapeCircles<N>::VehicleShapeCircles(VehicleShape _shape){
        len = _shape.len, wid=_shape.wid, reardist = _shape.reardist, wheelbase = _shape.wheelbase;
        const int &nums = N;
        CircleCenters.resize(nums);
        double stepsize = len/nums;
        Radius = std::sqrt(stepsize*stepsize+wid*wid)/2;
        CircleCenters[0] = stepsize/2-reardist;
        for(int i=1;i<nums;i++){
            CircleCenters[i] = CircleCenters[i-1]+stepsize;
        }
    }

    template<int N>
    std::vector<Points::PosPoint2D> VehicleShapeCircles<N>::getCircleCenters(const Points::Pos2D & CurPos){
        std::vector<Points::PosPoint2D> ans;
        ans.resize(CircleCenters.size());
        auto cosval = std::cos(CurPos.phi);
        auto sinval = std::sin(CurPos.phi);
        for(auto i = 0;i<ans.size();i++){
            ans[i].x = CurPos.x + CircleCenters[i]*cosval;
            ans[i].y = CurPos.y + CircleCenters[i]*sinval;
        }
        return ans;
    }

    typedef VehicleShapeCircles<2> VehicleShape2Circles;
    typedef VehicleShapeCircles<3> VehicleShape3Circles;

} // namespace Models