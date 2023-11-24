#ifndef WORLD_H
#define WORLD_H

#include "emp/Evolve/World.hpp"
#include "emp/data/DataFile.hpp"
#include "emp/math/Random.hpp"
#include "emp/math/random_utils.hpp"

#include "Org.h"

class OrgWorld : public emp::World<Organism> {

    emp::Random& random;
    emp::Ptr<emp::Random> random_ptr;
    emp::Ptr<emp::DataMonitor<int>> data_node_org_count;
    emp::Ptr<emp::DataMonitor<double, emp::data::Histogram>>
        data_node_cooperation_probs;

  public:
    OrgWorld(emp::Random& _random)
        : emp::World<Organism>(_random), random(_random) {
        random_ptr.New(_random);
        data_node_org_count = new emp::DataMonitor<int>();
        data_node_cooperation_probs = new emp::DataMonitor<double>();
    }

    ~OrgWorld() {
        if (data_node_org_count) {
            data_node_org_count.Delete();
        }
    }
    emp::DataMonitor<int>& GetOrgCountDataNode() {
        if (!data_node_org_count) {
            data_node_org_count.New();
            OnUpdate([this](size_t) {
                data_node_org_count->Reset();
                for (size_t i = 0; i < pop.size(); i++)
                    if (IsOccupied(i))
                        data_node_org_count->AddDatum(1);
            });
        }
        return *data_node_org_count;
    }
    emp::DataMonitor<double, emp::data::Histogram>&
    GetCooperationProbDataNode() {
        if (!data_node_cooperation_probs) {
            data_node_cooperation_probs.New();
            OnUpdate([this](size_t) {
                data_node_cooperation_probs->Reset();
                for (size_t i = 0; i < pop.size(); i++)
                    if (IsOccupied(i))
                        data_node_cooperation_probs->AddDatum(
                            pop[i]->GetCooperationProb());
            });
        }
        return *data_node_cooperation_probs;
    }

    emp::DataFile& SetupOrgCtFile(const std::string& filename) {
        auto& file = SetupFile(filename);
        auto& node1 = GetOrgCountDataNode();
        file.AddVar(update, "update", "Update");
        file.AddTotal(node1, "count", "Total number of orgs");
        file.PrintHeaderKeys();

        return file;
    }
    emp::DataFile& SetupCoopProbFile(const std::string& filename) {
        auto& file = SetupFile(filename);
        auto& node1 = GetCooperationProbDataNode();
        node1.SetupBins(0.0, 1.0, 21);
        file.AddVar(update, "update", "Update");
        file.AddMean(node1, "mean", "Mean cooperation probability");
        file.AddHistBin(node1, 0, "Hist_-1",
                        "Count for histogram bin -1 to <-0.9");
        file.AddHistBin(node1, 1, "Hist_-0.9",
                        "Count for histogram bin -0.9 to <-0.8");
        file.AddHistBin(node1, 2, "Hist_-0.8",
                        "Count for histogram bin -0.8 to <-0.7");
        file.AddHistBin(node1, 3, "Hist_-0.7",
                        "Count for histogram bin -0.7 to <-0.6");
        file.AddHistBin(node1, 4, "Hist_-0.6",
                        "Count for histogram bin -0.6 to <-0.5");
        file.AddHistBin(node1, 5, "Hist_-0.5",
                        "Count for histogram bin -0.5 to <-0.4");
        file.AddHistBin(node1, 6, "Hist_-0.4",
                        "Count for histogram bin -0.4 to <-0.3");
        file.AddHistBin(node1, 7, "Hist_-0.3",
                        "Count for histogram bin -0.3 to <-0.2");
        file.AddHistBin(node1, 8, "Hist_-0.2",
                        "Count for histogram bin -0.2 to <-0.1");
        file.AddHistBin(node1, 9, "Hist_-0.1",
                        "Count for histogram bin -0.1 to <0.0");
        file.AddHistBin(node1, 10, "Hist_0.0",
                        "Count for histogram bin 0.0 to <0.1");
        file.AddHistBin(node1, 11, "Hist_0.1",
                        "Count for histogram bin 0.1 to <0.2");
        file.AddHistBin(node1, 12, "Hist_0.2",
                        "Count for histogram bin 0.2 to <0.3");
        file.AddHistBin(node1, 13, "Hist_0.3",
                        "Count for histogram bin 0.3 to <0.4");
        file.AddHistBin(node1, 14, "Hist_0.4",
                        "Count for histogram bin 0.4 to <0.5");
        file.AddHistBin(node1, 15, "Hist_0.5",
                        "Count for histogram bin 0.5 to <0.6");
        file.AddHistBin(node1, 16, "Hist_0.6",
                        "Count for histogram bin 0.6 to <0.7");
        file.AddHistBin(node1, 17, "Hist_0.7",
                        "Count for histogram bin 0.7 to <0.8");
        file.AddHistBin(node1, 18, "Hist_0.8",
                        "Count for histogram bin 0.8 to <0.9");
        file.AddHistBin(node1, 19, "Hist_0.9",
                        "Count for histogram bin 0.9 to 1.0");
        file.PrintHeaderKeys();

        return file;
    }
    void Update() {
        emp::World<Organism>::Update();

        emp::vector<size_t> schedule = emp::GetPermutation(random, GetSize());
        std::cout << "Updating!" << std::endl;
        for (int i : schedule) {
            if (!IsOccupied(i)) {
                continue;
            }
            pop[i]->Process(100);
        }

        schedule = emp::GetPermutation(random, GetSize());
        for (int i : schedule) {
            if (!IsOccupied(i)) {
                continue;
            }
            emp::Ptr<Organism> offspring = pop[i]->CheckReproduction();
            if (offspring) {
                DoBirth(*offspring, i);
            }
        }
    }
};
#endif