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
    auto lai = new OutputWrapper<Individual, double>("lai", "patch-level lai for this individual or cohort (function fpar)", "", [] (const Individual* individual) -> double {
        return individual->lai;
    });
    auto lai_layer = new OutputWrapper<Individual, double>("lai_layer", "patch-level lai for cohort in current vertical layer (function fpar)", "", [] (const Individual* individual) -> double {
        return individual->lai_layer;
    });
    auto lai_indiv = new OutputWrapper<Individual, double>("lai_indiv", "individual leaf area index (individual and cohort modes only)", "", [] (const Individual* individual) -> double {
        return individual->lai_indiv;
    });
    auto lai_daily = new OutputWrapper<Individual, double>("lai_daily", "patch-level individual leaf area index (individual and cohort modes only)", "", [] (const Individual* individual) -> double {
        return individual->lai_daily;
    });
    auto lai_indiv_daily = new OutputWrapper<Individual, double>("lai_indiv_daily", "daily individual leaf area index (individual and cohort modes only)", "", [] (const Individual* individual) -> double {
        return individual->lai_indiv_daily;
    });
    registry->register_output(vc_max);
    registry->register_output(agd_d);
    registry->register_output(adtmm);
    registry->register_output(rd_g);
    registry->register_output(je);
    registry->register_output(nactive_opt);
    registry->register_output(vmaxnlim);
    registry->register_output(lai);
    registry->register_output(lai_layer);
    registry->register_output(lai_indiv);
    registry->register_output(lai_daily);
    registry->register_output(lai_indiv_daily);
    
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

ObjectOutputRegistryBase<double>* get_met_outputs() {
    ObjectOutputRegistry<Climate, double>* registry = new ObjectOutputRegistry<Climate, double>("Today's met data", get_climate, list_climate_names);
	auto temp = new OutputWrapper<Climate, double>("temp", "mean air temperature today (°C)", "", [] (const Climate* climate) -> double {
        return climate->temp;
    });
	auto rad = new OutputWrapper<Climate, double>("rad", "total daily net downward shortwave solar radiation today", "J/m2/day", [] (const Climate* climate) -> double {
        return climate->rad;
    });
	auto par = new OutputWrapper<Climate, double>("par", "total daily photosynthetically-active radiation today", "J/m2/day", [] (const Climate* climate) -> double {
        return climate->par;
    });
	auto par_umol = new OutputWrapper<Climate, double>("par_umol", "total daily photosynthetically-active radiation today", "umol/m2/s", [] (const Climate* climate) -> double {
        return climate->par_umol;
    });
	auto prec = new OutputWrapper<Climate, double>("prec", "precipitation today", "mm", [] (const Climate* climate) -> double {
        return climate->prec;
    });
	auto u10 = new OutputWrapper<Climate, double>("u10", "10 m wind", "km/h", [] (const Climate* climate) -> double {
        return climate->u10;
    });
	auto relhum = new OutputWrapper<Climate, double>("relhum", "rel. humidity", "fract.", [] (const Climate* climate) -> double {
        return climate->relhum;
    });
	auto tmin = new OutputWrapper<Climate, double>("tmin", "min and max daily temperature", "°C", [] (const Climate* climate) -> double {
        return climate->tmin;
    });
    auto tmax = new OutputWrapper<Climate, double>("tmax", "max daily temperature", "°C", [] (const Climate* climate) -> double {
        return climate->tmax;
    });
	auto daylength = new OutputWrapper<Climate, double>("daylength", "day length today", "h", [] (const Climate* climate) -> double {
        return climate->daylength;
    });
	auto co2 = new OutputWrapper<Climate, double>("co2", "atmospheric ambient CO2 concentration today", "ppmv", [] (const Climate* climate) -> double {
        return climate->co2;
    });
	auto lat = new OutputWrapper<Climate, double>("lat", "latitude", "degrees; +=north, -=south", [] (const Climate* climate) -> double {
        return climate->lat;
    });
	auto insol = new OutputWrapper<Climate, double>("insol", "Insolation today, units defined by instype", "", [] (const Climate* climate) -> double {
        return climate->insol;
    });
	auto instype = new OutputWrapper<Climate, double>("instype", "Type of insolation. This decides how to interpret the variable insol, see also documentation for the insoltype enum.", "", [] (const Climate* climate) -> double {
        return climate->instype;
    });
	auto eet = new OutputWrapper<Climate, double>("eet", "equilibrium evapotranspiration today", "mm/day", [] (const Climate* climate) -> double {
        return climate->eet;
    });
	auto mtemp = new OutputWrapper<Climate, double>("mtemp", "mean temperature for the last 31 days", "°C", [] (const Climate* climate) -> double {
        return climate->mtemp;
    });
	auto mtemp_min20 = new OutputWrapper<Climate, double>("mtemp_min20", "mean of lowest mean monthly temperature for the last 20 years", "°C", [] (const Climate* climate) -> double {
        return climate->mtemp_min20;
    });
	auto mtemp_max20 = new OutputWrapper<Climate, double>("mtemp_max20", "mean of highest mean monthly temperature for the last 20 years", "°C", [] (const Climate* climate) -> double {
        return climate->mtemp_max20;
    });
	auto mtemp_max = new OutputWrapper<Climate, double>("mtemp_max", "highest mean monthly temperature for the last 12 months", "°C", [] (const Climate* climate) -> double {
        return climate->mtemp_max;
    });
	auto gdd5 = new OutputWrapper<Climate, double>("gdd5", "accumulated growing degree day sum on 5 degree base (reset when temperatures fall below 5 °C)", "dd?", [] (const Climate* climate) -> double {
        return climate->gdd5;
    });
	auto agdd5 = new OutputWrapper<Climate, double>("agdd5", "total gdd5 (accumulated) for this year (reset 1 January)", "dd?", [] (const Climate* climate) -> double {
        return climate->agdd5;
    });
	auto gdd0 = new OutputWrapper<Climate, double>("gdd0", "accumulated growing degree day sum on 0 degree base (Wolf et al. 2008)", "dd?", [] (const Climate* climate) -> double {
        return climate->gdd0;
    });
	auto agdd0 = new OutputWrapper<Climate, double>("agdd0", "total gdd0 (accumulated) for this year (reset 1 January)", "dd?", [] (const Climate* climate) -> double {
        return climate->agdd0;
    });
	auto gtemp = new OutputWrapper<Climate, double>("gtemp", "Respiration response to today's air temperature incorporating damping of Q10 due to temperature acclimation (Lloyd & Taylor 1994)", "", [] (const Climate* climate) -> double {
        return climate->gtemp;
    });
    registry->register_output(temp);
    registry->register_output(rad);
    registry->register_output(par);
    registry->register_output(par_umol);
    registry->register_output(prec);
    registry->register_output(u10);
    registry->register_output(relhum);
    registry->register_output(tmin);
    registry->register_output(tmax);
    registry->register_output(daylength);
    registry->register_output(co2);
    registry->register_output(lat);
    registry->register_output(insol);
    registry->register_output(instype);
    registry->register_output(eet);
    registry->register_output(mtemp);
    registry->register_output(mtemp_min20);
    registry->register_output(mtemp_max20);
    registry->register_output(mtemp_max);
    registry->register_output(gdd5);
    registry->register_output(agdd5);
    registry->register_output(gdd0);
    registry->register_output(agdd0);
	registry->register_output(gtemp);
	return registry;
}
