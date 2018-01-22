#pragma once
#include <vector>
#include "StringOP.h"
#include "Matrix.h"
#include "Vector.h"
#include "Reaction.h"
#include "BTCSet.h"
#include "Function.h"
#include "Solid_Phase.h"
#include "RxnNetwork.h"
#include "Buildup.h"
#include "EnvExchange.h"
#include "Evaporation.h"

#ifndef n_flow_params
#define n_flow_params 40
#endif

using namespace std;

enum Block_types { Soil, Pond, Storage, Catchment, Manhole, Darcy, Stream, Plant };
enum basic_properties { _H = 1, _A = 2, _V = 3, _S = 4, _z0 = 5, _depth = 6, _q = 8, _relative_saturation = 9, _moisture_content = 10, _depression_storage = 12, _vapor_diffusion = 13, _bulk_density = 14, _air_volume = 15, location_x=20, location_y=21, location_z=22 };
class CGWA;
class CMedium;

struct plant_props // plant properties
{
	double LAI; // Leaf area index
	vector<string> limiting_nutrients; // the list of limiting nutrients
	vector<double> half_saturation_constants; //half saturation factors corresponding to each limiting nutrient
	int look_up_limiting_nutrient(string s)
	{
		for (int i = 0; i < limiting_nutrients.size(); i++)
			if (s == limiting_nutrients[i])
				return i;
		return -1;
	}
	CStringOP plant_growth_rate_expression;
	CStringOP LAI_growth_rate_expression; 

};

struct r_location
{
	double x;
	double y;
	double z;
};

class CMBBlock
{
public:
	CMBBlock(void);// default constructor
	~CMBBlock(void);
	CMBBlock(const CMBBlock& BB);// copy constructor
	CMBBlock(string name, string params); //builds a block and assigns the properties in the string
	int n_constts; 	int n_phases; //number of chemical species, number of phases;
    void set_num_phases_constts(int n, int m);
    void set_num_phases(int n);
    void set_num_constts(int m);
    CMBBlock& operator=(const CMBBlock &BB); //equal operator
	string ID; //Identification of the Mass Balance Block
	void set_location(double x, double y=0, double z=0) { real_location.x = x, real_location.y = y, real_location.z = z; };
	void set_location_x(double x) { real_location.x = x; };
	void set_location_y(double y) { real_location.y = y; };
	void set_location_z(double z) { real_location.z = z; };
	double get_x() { return real_location.x; };
	double get_y() { return real_location.y; };
	double get_z() { return real_location.z; };
	vector<double> get_location()
	{
		vector<double> out; out.push_back(real_location.x); out.push_back(real_location.y); out.push_back(real_location.z);
		return out; 
	}

	vector<CSolid_Phase*> Solid_phase;
	vector<vector<double>> G; //Solid phases G[i][j] i: solid_type_counter, j: phase identifier
	vector<vector<double>> G_star; //Solid phases
	vector<vector<double>> CG; // Sorbed Phases
	vector<vector<double>> CG_star; // Sorbed Phases
	vector<vector<double>> G_stored_mass; //Solid phases G[i][j] i: solid_type_counter, j: phase identifier
	vector<vector<double>> CG_stored_mass; // Sorbed Phases
	vector<double> fs_params; //hydraulics_parameters;
	vector<double> rxn_params; //Reaction_parameters
	vector<vector<double>> phi; //Capacities
	vector<CVector> capacity_c; // colloid capacities
	vector<CVector> capacity_c_star;
	vector<CVector> capacity_c_Q; // constituent capacity
	vector<CVector> capacity_c_star_Q;
    void evaluate_capacity_c();
    void evaluate_capacity_c_star();
	vector<CMatrix> K; //exchange of phases
	vector<CMatrix> K_star; //exchange of phases
	CMatrix f; //interfacial surface area between phases
	vector<CMatrix> kappa; //contaminant mass exchange between phases
	CStringOP H_S_expression; //Storage-Head Relationship
	string H_S_expression_txt;
	CStringOP V_S_expression; //Storage-Volume Relationship
    double calc_rxn_rate(CStringOP &C); //calculate reaction rate
    vector<double> calc_rxn_prod_rate(); //calculate production rate of all constituents
    double get_rate_exchange(int particle_type, int constituent);
    double get_exchange_rate_star(int particule_type, int phase, int constituent);
    double calc(CStringOP &C, vector<int> ii); //The function to calculate any expression
    double calc_star(CStringOP &C, vector<int> ii); //The function to calculate any expression based on star values
    double calc(CStringOP &C); //The function to calculate any expression
    double calc_star(CStringOP &C); //The function to calculate any expression based on star values
    double get_val(int i, vector<int> ii);// get the value of physical properties, variables and parameters
    double get_val_star(int i, vector<int>); //get the value of physical properties, variables and parameters based on star values
    double get_val(int i);// get the value of physical properties, variables and parameters
    double get_val_star(int i); //get the value of physical properties, variables and parameters based on star values
    double get_val(string SS); //get the value of physical properties, variables and parameters
    void set_val(int i, double val);// set the value of physical properties, variables and parameters
    void set_val_star(int i, double val); //set the value of physical properties, variables and parameters based on star values
    void set_val(const string &SS, double val); //set the value of physical properties, variables and parameters based on star values
	vector<CBTCSet> inflow; //inflow time-series
	double& get_S() { return S; };
	double& get_S_star() { return S_star; };
	
	vector<string> inflow_filename;
	int indicator; //specify block medium 0: soil, 1: pond
	vector<int> connectors; //the id of connectors attached to the block
	vector<int> connectors_se;// 0: block1 of the connector 1: block 2 of the connector
    void get_funcs(CStringOP &term);
    void evaluate_functions(int i);
    void evaluate_functions();
	double vapor_diffusion;
	bool fixed_evaporation;
	double fixed_evaporation_val;
	int setzero;
	double outflow_corr_factor;
    double get_evaporation(double t);
    double get_evaporation(int j, double t);
	vector<int> Solid_phase_id;
	vector<CFunction> funcs;
	double MBBlocks;  //MM
	bool fixed;// fixed flow for connected connector
    void evaluate_K();
    void evaluate_K_star();
    void evaluate_capacity();
    void evaluate_capacity_star();
	vector<string> initial_g_counter_p; vector<string> initial_g_counter_l; vector<double> initial_g;
	vector<string> initial_cg_counter_p; vector<string> initial_cg_counter_l; vector<string> initial_cg_counter_c;  vector<double> initial_cg;
    int get_tot_num_phases();
	CRxnNetwork *RXN;
    double get_exchange_rate(int particule_type, int phase, int constituent);
    double get_kd(int particule_type, int phase, int constituent);
    void set_CG(int particle_type, int phase, int constituent, double val);
    double &_CG(int particle_type, int phase, int constituent);
    double get_CG(int particle_type, int phase, int constituent);
    int get_member_no(int solid_id, int phase_no);
    int get_member_no(int solid_id, int phase_no, int constituent);
    double get_reaction_rate(int i, CVector &X, bool star);
    CVector get_reaction_rates(CVector &X, bool star);
    double get_reaction_rate(int i, bool star);
    CVector get_reaction_rates(bool star);
    CVector get_rxn_change(bool star);
    CVector get_rxn_change(CVector &X, bool star);
    CVector get_X_from_CG();
    void set_CG_star(const CVector &X);
    CMatrix Eval_Stoichiometry(bool star);
	bool precipitation_swch;
	bool light_swch;
	vector<CBuildup*> buildup;
	vector<string> buildup_id;
	vector<CEnvExchange*> envexchange;
	vector<string> envexchange_id;
	vector<CEvaporation*> evaporation_m;
	vector<string> evaporation_id;
    int lookup_particle_type(string S);
    int lookup_env_exchange(string S);
	vector<CBTC> *light;
	vector<CBTC> *temperature;

	double *current_temperature;
	double *current_light;
	double *current_humidity;
	double *current_wind;
	double light_reduction_factor;
	bool perform_rxn = true;
	plant_props plant_prop; //plant properties including LAI and growth parameters. 
	void set_up_plant_growth_expressions(); //The plant growth expressions are built. 
#ifdef GIFMOD
	CMedium *parent; 
#endif
#ifdef GWA
	CGWA *parent;
#endif
	int air_phase;


private: 
	double H, A, V, S;  //Head, Area, Volume, Storage
	double q; // Darcy flux
	double DS;  //Depression Storage (thickness)
	double H_star, A_star, V_star, S_star; //Head, Area, Volume, Storage
	double z0; //reference_elevation
	double bulk_density;
	double porosity;
	r_location real_location; 

};

