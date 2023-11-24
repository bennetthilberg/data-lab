#ifndef ORG_H
#define ORG_H

#include "emp/math/Random.hpp"
#include "emp/tools/string_utils.hpp"

class Organism {
    private:
        double points;
        double cooperation_prob;
        emp::Ptr<emp::Random> random;

    public:
    Organism(emp::Ptr<emp::Random> _random, double _points=0.0, double _cooperation_prob=0.5) :
        points(_points), random(_random), cooperation_prob(_cooperation_prob) {;}

    void SetPoints(double _in) {points = _in;}
    void AddPoints(double _in) {points += _in;}
    double GetCooperationProb() {return cooperation_prob;}
    void SetCooperationProb(double _in) {cooperation_prob = _in;}

    emp::Ptr<Organism> CheckReproduction() {
        if(points>1000) {
            emp::Ptr<Organism> offspring = new Organism(*this);
            offspring->SetPoints(0);
            offspring->Mutate(*random);
            AddPoints(-1000);

            return offspring;
        }
        return nullptr;
    }
    void Mutate(emp::Random rand){
        cooperation_prob += rand.GetRandNormal(0, 0.05);
    }

    void Process(int _points) {
        AddPoints(_points);
        std::cout << "Processing" << std::endl;
       
    }


};
#endif