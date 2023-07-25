/*********************************************************************
 * Author : BorisVandermeer
 * 
 * Discription ：
 *      A Simple implement of Trajectory
 * 
*********************************************************************/
#pragma once

#include<vector>
#include<memory>

#include<Points/PointsType.h>
#include<Interplot/SplineCurve.h>

namespace PNC_Common{
    class PathSegment{
    public:
        PathSegment() = default;
        typedef std::shared_ptr<Interplot::SplineCurve>  CurvePtr;
        enum PathSegmentType{
            Forward, BackWard,
        };
        typedef Points::PosPoint2D Point2D;
        typedef Points::Pos2D Pos2D;
        typedef Points::PosPoint2Ds RefPoints;

        void SetCurve(CurvePtr ptr){
                Data = ptr;
                FromHeading=Data->getHeading(0);ToHeading = Data->getHeading(Data->max_s);
                length = Data->max_s;
            }
        void SetCurve(RefPoints & points){
                Data = std::make_shared<Interplot::SplineCurve>();   Data->setPoints(points);
                FromHeading=Data->getHeading(0);ToHeading = Data->getHeading(Data->max_s);
                length = Data->max_s;
            }
        void SetCurve(RefPoints & points, double rebuildsize){
                Data = std::make_shared<Interplot::SplineCurve>();   Data->setPoints(points);
                FromHeading=Data->getHeading(0);ToHeading = Data->getHeading(Data->max_s);
                Data->rebuild(rebuildsize);
                length = Data->max_s;
            }
        void SetCurve(RefPoints & points,double _FromHeading, double _ToHeading){
                Data = std::make_shared<Interplot::SplineCurve>();  
                Data->setPoints(points,_FromHeading,_ToHeading);
                FromHeading=_FromHeading;ToHeading=_ToHeading;
                length = Data->max_s;
            }
        Point2D operator() (double s) const {return Data->operator()(s);};
        double getDirection(double s) const {return Data->getHeading(s);}
        Pos2D  getPos(double s) const {auto tmp = Data->operator()(s); return Pos2D(tmp.x,tmp.y,Data->getHeading(s));}
        // return refpos's nearrest point on line.
        double getProjection(Point2D refpos,double min_s,double max_s) const {return Data->getProjection(refpos,max_s,min_s);}
        double getProjection(Pos2D refpos,double min_s,double max_s) const {return Data->getProjection(Point2D(refpos.x,refpos.y),max_s,min_s);}

        PathSegmentType Type;
        double length;
        
        CurvePtr Data;
        double FromHeading;
        double ToHeading;
    };

    class DefaultSpeedProfile{
    public : 
        double operator()(double s) const {return Data->operator()(s);}
    public:
        std::shared_ptr<Interplot::Spline> Data;
    };

    template<class SpeedProfile = DefaultSpeedProfile>
    class TrajectorySegmentTemp : public PathSegment{
        // typedef DefaultSpeedProfile SpeedProfile;
    public:
        TrajectorySegmentTemp() = default;
        TrajectorySegmentTemp(SpeedProfile data){SpeedData = data;}
        void setSpeed(SpeedProfile data){SpeedData = data;}
        double getSpeed(double s){return SpeedData(s);}
    private:
        SpeedProfile SpeedData;
    };

    typedef TrajectorySegmentTemp<DefaultSpeedProfile> TrajectorySegment;

} // namespace PNC_Common