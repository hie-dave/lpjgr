#include "init.h"
#include "model_access.h"
#include "output_registry.h"
#include "output_wrapper.h"
#include "state.h"

#include "guess.h"

OutputRegistry<PhotosynthesisResult, double>* photosynthesis_outputs;

void register_outputs() {
    if (photosynthesis_outputs == NULL) {
        photosynthesis_outputs = new OutputRegistry<PhotosynthesisResult, double>("Photosynthesis SPFT Outputs");
    }
    auto vc_max = new OutputWrapper<PhotosynthesisResult, double>("vm", "RuBisCO capacity", "gC/m2/day", [] (const PhotosynthesisResult* photosynthesis) -> double {
        return photosynthesis->vm;
    });
    auto agd_d = new OutputWrapper<PhotosynthesisResult, double>("agd_d", "gross daily photosynthesis", "gC/m2/day", [] (const PhotosynthesisResult* photosynthesis) -> double {
        return photosynthesis->agd_g;
    });
    auto adtmm = new OutputWrapper<PhotosynthesisResult, double>("adtmm", "leaf-level net daytime photosynthesis in CO2 diffusion units", "mm/m2/day", [] (const PhotosynthesisResult* photosynthesis) -> double {
        return photosynthesis->adtmm;
    });
    auto rd_g = new OutputWrapper<PhotosynthesisResult, double>("rd_g", "leaf respiration", "gC/m2/day", [] (const PhotosynthesisResult* photosynthesis) -> double {
        return photosynthesis->rd_g;
    });
    auto je = new OutputWrapper<PhotosynthesisResult, double>("je", "PAR-limited photosynthesis rate", "gC/m2/h", [] (const PhotosynthesisResult* photosynthesis) -> double {
        return photosynthesis->je;
    });
    auto nactive_opt = new OutputWrapper<PhotosynthesisResult, double>("nactive_opt", "optimal leaf nitrogen associated with photosynthesis", "kgN/m2", [] (const PhotosynthesisResult* photosynthesis) -> double {
        return photosynthesis->nactive_opt;
    });
    auto vmaxnlim = new OutputWrapper<PhotosynthesisResult, double>("vmaxnlim", "nitrogen limitation on vm", "?", [] (const PhotosynthesisResult* photosynthesis) -> double {
        return photosynthesis->vmaxnlim;
    });
    photosynthesis_outputs->register_output(vc_max);
    photosynthesis_outputs->register_output(agd_d);
    photosynthesis_outputs->register_output(adtmm);
    photosynthesis_outputs->register_output(rd_g);
    photosynthesis_outputs->register_output(je);
    photosynthesis_outputs->register_output(nactive_opt);
    photosynthesis_outputs->register_output(vmaxnlim);
}

// [[Rcpp::export]]
double get_spft_numeric_output(std::string pft_name, std::string name) {
    ensure_initialised();
    if (photosynthesis_outputs->contains_output(name)) {
        PhotosynthesisResult* photosynthesis = get_photosynthesis(pft_name);
        return photosynthesis_outputs->get_output(photosynthesis, name);
    }

    std::string err_msg = "Cannot find numeric output with name: '" + name + "'";
    throw std::runtime_error(err_msg);
}

// [[Rcpp::export]]
Rcpp::CharacterVector list_spft_numeric_outputs() {
    ensure_initialised();
    std::vector<std::string> outputs = photosynthesis_outputs->get_output_names();

    Rcpp::CharacterVector r_outputs(outputs.size());
    for (int i = 0; i < outputs.size(); i++) {
        r_outputs[i] = outputs[i];
    }

    return r_outputs;
}

// [[Rcpp::export]]
std::string get_output_info(std::string output_name) {
    ensure_initialised();
    if (photosynthesis_outputs->contains_output(output_name)) {
        return photosynthesis_outputs->get_output_info(output_name);
    }
    throw std::runtime_error("Unknown output: '" + output_name + "'");
}
