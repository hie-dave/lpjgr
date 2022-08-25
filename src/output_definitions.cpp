#include "output_definitions.h"
#include "output_registry.h"
#include "object_output_registry.h"
#include "model_access.h"

ObjectOutputRegistryBase<double>* get_individual_outputs() {
    ObjectOutputRegistry<Individual, double>* registry = new ObjectOutputRegistry<Individual, double>("Individual outputs", get_individual, list_individuals);

    // Photosynthesis
    auto vc_max = new OutputWrapper<Individual, double>("vm", "RuBisCO capacity", "gC/m2/day", [] (const Individual* individual) -> double {
        return individual->photosynthesis.vm;
    });
    auto agd_d = new OutputWrapper<Individual, double>("agd_d", "gross daily photosynthesis", "gC/m2/day", [] (const Individual* individual) -> double {
        return individual->photosynthesis.agd_g;
    });
    auto adtmm = new OutputWrapper<Individual, double>("adtmm", "leaf-level net daytime photosynthesis in CO2 diffusion units", "mm/m2/day", [] (const Individual* individual) -> double {
        return individual->photosynthesis.adtmm;
    });
    auto rd_g = new OutputWrapper<Individual, double>("rd_g", "leaf respiration", "gC/m2/day", [] (const Individual* individual) -> double {
        return individual->photosynthesis.rd_g;
    });
    auto je = new OutputWrapper<Individual, double>("je", "PAR-limited photosynthesis rate", "gC/m2/h", [] (const Individual* individual) -> double {
        return individual->photosynthesis.je;
    });
    auto nactive_opt = new OutputWrapper<Individual, double>("nactive_opt", "optimal leaf nitrogen associated with photosynthesis", "kgN/m2", [] (const Individual* individual) -> double {
        return individual->photosynthesis.nactive_opt;
    });
    auto vmaxnlim = new OutputWrapper<Individual, double>("vmaxnlim", "nitrogen limitation on vm", "?", [] (const Individual* individual) -> double {
        return individual->photosynthesis.vmaxnlim;
    });
    registry->register_output(vc_max);
    registry->register_output(agd_d);
    registry->register_output(adtmm);
    registry->register_output(rd_g);
    registry->register_output(je);
    registry->register_output(nactive_opt);
    registry->register_output(vmaxnlim);

    return registry;
}

ObjectOutputRegistryBase<double>* get_spft_outputs() {
    ObjectOutputRegistry<Standpft, double>* registry = new ObjectOutputRegistry<Standpft, double>("Stand PFT Outputs", get_standpft, list_spft_names);
    auto vc_max = new OutputWrapper<Standpft, double>("vm", "RuBisCO capacity", "gC/m2/day", [] (const Standpft* stand) -> double {
        return stand->photosynthesis.vm;
    });
    auto agd_d = new OutputWrapper<Standpft, double>("agd_d", "gross daily photosynthesis", "gC/m2/day", [] (const Standpft* stand) -> double {
        return stand->photosynthesis.agd_g;
    });
    auto adtmm = new OutputWrapper<Standpft, double>("adtmm", "leaf-level net daytime photosynthesis in CO2 diffusion units", "mm/m2/day", [] (const Standpft* stand) -> double {
        return stand->photosynthesis.adtmm;
    });
    auto rd_g = new OutputWrapper<Standpft, double>("rd_g", "leaf respiration", "gC/m2/day", [] (const Standpft* stand) -> double {
        return stand->photosynthesis.rd_g;
    });
    auto je = new OutputWrapper<Standpft, double>("je", "PAR-limited photosynthesis rate", "gC/m2/h", [] (const Standpft* stand) -> double {
        return stand->photosynthesis.je;
    });
    auto nactive_opt = new OutputWrapper<Standpft, double>("nactive_opt", "optimal leaf nitrogen associated with photosynthesis", "kgN/m2", [] (const Standpft* stand) -> double {
        return stand->photosynthesis.nactive_opt;
    });
    auto vmaxnlim = new OutputWrapper<Standpft, double>("vmaxnlim", "nitrogen limitation on vm", "?", [] (const Standpft* stand) -> double {
        return stand->photosynthesis.vmaxnlim;
    });
    registry->register_output(vc_max);
    registry->register_output(agd_d);
    registry->register_output(adtmm);
    registry->register_output(rd_g);
    registry->register_output(je);
    registry->register_output(nactive_opt);
    registry->register_output(vmaxnlim);

    return registry;
}
