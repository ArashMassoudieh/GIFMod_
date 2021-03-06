#include "stdafx.h"
#include "Connection.h"
//QUI
#include "enums.h"
#include "Medium.h"


CConnection::CConnection(void)
{
	fixed = false;
	A=0;
	d=0;
	v=0;
	A_star=0;
	Q_star=0;
	v_star=0;
	Q=0;
	flow_factor=1;
	Q_v=0;
	Q_v_star=0;
	flow_params.resize(n_flow_params);
	flow_params[z1] = -1000000;
	flow_params[z2] = -1000000;
	flow_params[flow_exponent] = 0.6667;
	dispersivity = 0;
	dispersion_expression = string("f[14]*f[7]/(f[2]+0.00000001)");
	dispersion_strng = "f[14]*f[7]/f[2]";
	settling = -1;
	const_area = true;
	presc_flow = false;
	control = false;
	vapor_transport = false;
}

CConnection::CConnection(string s)
{
	fixed = false;
	A=0;
	d=0;
	v=0;
	A_star=0;
	Q_star=0;
	v_star=0;
	Q=0;
	flow_factor=1;
	Q_v=0;
	Q_v_star=0;
	flow_params.resize(n_flow_params);
	flow_params[z1] = -1000000;
	flow_params[z2] = -1000000;
	flow_params[flow_exponent] = 0.6667;
	dispersivity = 0;
	dispersion_expression = string("f[14]*f[7]/(f[2]+0.00000001)");
	dispersion_strng = "f[14]*f[7]/f[2]";
	settling = -1;
	const_area = true;
	presc_flow = false;
	control = false;
    vapor_transport = false;
#ifndef QT_version
	showmessages = true;
#endif // QT_version
	set_properties(s);
}



CConnection::~CConnection(void)
{
	for (unsigned int i=0; i<Solid_phase.size(); i++) Solid_phase[i] = NULL;
	Block1 = NULL;
	Block2 = NULL;
	RXN = NULL;
	Controller = NULL;
}

CConnection::CConnection(const CConnection &CC)
{
	Block1ID = CC.Block1ID;
	Block2ID = CC.Block2ID;
	Block1 = CC.Block1;
	Block2 = CC.Block2;
	Block1N = CC.Block1N;
	Block2N = CC.Block2N;
	A = CC.A;
	d = CC.d;
	v = CC.v;
	v_star = CC.v_star;
	A_star = CC.A_star;
	c_dispersion = CC.c_dispersion;
	c_dispersion_star = CC.c_dispersion_star;
	Q_star = CC.Q_star;
	Q_v = CC.Q_v;
	flow_params = CC.flow_params;
	Q = CC.Q;
	Q_v_star = CC.Q_v_star;
	dispersion = CC.dispersion;
	dispersion_star = CC.dispersion_star;
	Solid_phase_id = CC.Solid_phase_id;
	Solid_phase = CC.Solid_phase;
	flow_expression = CC.flow_expression;
	flow_expression_strng = CC.flow_expression_strng;
	fixed = CC.fixed;
	fixed_val = CC.fixed_val;
	funcs = CC.funcs;
	flow_factor=CC.flow_factor;
	flow_expression_v = CC.flow_expression_v;
	flow_expression_strng_v = CC.flow_expression_strng_v;
	dispersivity = CC.dispersivity;
	dispersion_expression = CC.dispersion_expression;
	dispersion_strng = CC.dispersion_strng;
	area_expression = CC.area_expression;
	area_expression_strng = CC.area_expression_strng;
	settling = CC.settling;
	const_area = CC.const_area;
	ID = CC.ID;
	presc_flow = CC.presc_flow;
	pre_flow_filename = CC.pre_flow_filename;
	presc_flowrate = CC.presc_flowrate;
	control = CC.control;
	controller_id = CC.controller_id;
	vapor_transport = CC.vapor_transport;
}

CConnection& CConnection::operator=(const CConnection &CC)
{
	Block1ID = CC.Block1ID;
	Block2ID = CC.Block2ID;
	Block1 = CC.Block1;
	Block2 = CC.Block2;
	Block1N = CC.Block1N;
	Block2N = CC.Block2N;
	A = CC.A;
	d = CC.d;
	v = CC.v;
	v_star = CC.v_star;
	A_star = CC.A_star;
	c_dispersion = CC.c_dispersion;
	Q_star = CC.Q_star;
	Q_v = CC.Q_v;
	Q_v_star = CC.Q_v_star;
	flow_params = CC.flow_params;
	Q = CC.Q;
	Solid_phase_id = CC.Solid_phase_id;
	Solid_phase = CC.Solid_phase;
	dispersion_star = CC.dispersion_star;
	dispersion = CC.dispersion;
	c_dispersion_star = CC.c_dispersion_star;
	flow_expression = CC.flow_expression;
	flow_expression_strng = CC.flow_expression_strng;
	fixed = CC.fixed;
	fixed_val = CC.fixed_val;
	funcs = CC.funcs;
	flow_factor=CC.flow_factor;
	flow_expression_v = CC.flow_expression_v;
	flow_expression_strng_v = CC.flow_expression_strng_v;
	dispersivity = CC.dispersivity;
	dispersion_expression = CC.dispersion_expression;
	dispersion_strng = CC.dispersion_strng;
	area_expression = CC.area_expression;
	area_expression_strng = CC.area_expression_strng;
	settling = CC.settling;
	const_area = CC.const_area;
	ID = CC.ID;
	presc_flow = CC.presc_flow;
	pre_flow_filename = CC.pre_flow_filename;
	presc_flowrate = CC.presc_flowrate;
	control = CC.control;
	controller_id = CC.controller_id;
	vapor_transport = CC.vapor_transport;
	return *this;



}

double CConnection::calc(const CStringOP &term, int ii) //The function to calculate any expression
{
	if (term.function==true)
    {
        if (term.number == min_)
			return min(calc(term.terms[0],ii), calc(term.terms[1],ii));
		if (term.number == max_)
			return max(calc(term.terms[0],ii), calc(term.terms[1],ii));
		if (term.number == sq1_)
			return 0.5/calc(term.terms[1],ii)*(calc(term.terms[0],ii)*calc(term.terms[1],ii)+sqrt(pow(calc(term.terms[0],ii)*calc(term.terms[1],ii),2)+1));
        if (term.number == frs_)
			return funcs[0].evaluate(get_val_star(9));
		if (term.number == fas_)
			return funcs[0].evaluate(get_val_star(4));
		if (term.number==mon_)
			return mon(calc(term.terms[0],ii),calc(term.terms[1],ii));
		if (term.number == sq2_)
		{	double term1 = calc(term.terms[0],ii);
			int sign_ = sgn(term1);
			double term2 = calc(term.terms[1],ii);
			return double(sign_)*pow(fabs(term1),(0.5*term1+term2)/(term1+term2));
		}
		if (term.number == mo1_)
			return mon(calc(term.terms[0], ii), calc(term.terms[1], ii))*calc(term.terms[0], ii) + mon(-calc(term.terms[0], ii), calc(term.terms[1], ii))*calc(term.terms[0], ii);


    }
	double out = 0;
	if ((term.nterms == 1) && (term.nopts == 0))
	{
		if (term.parameter == true)
			out = 0.5*(Block1->rxn_params[term.number-2000]+Block2->rxn_params[term.number-2000]);
		else if (term.concentration == true)
			if (Q>0) out = Block1->CG[term.number][term.phase]; else out = Block2->CG[term.number][term.phase];
		else if (term.s_concentration == true)
			if (term.number == -1)
				if (Q>0) out = Block1->G[ii][term.phase]; else out = Block2->G[ii][term.phase];
			else
				if (Q>0) out = Block1->G[term.number][term.phase]; else out = Block2->G[term.number][term.phase];
		else if (term.physical_ch == true)
			out = get_val(term.number,ii);
		else if (term.constant == true)
			out = term.value;
		else if (term.s_block == true)
		{
			vector<int> jj; jj.push_back(ii);
			out = Block1->get_val(term.number, jj);
		}
		else if (term.t_block == true)
		{
			vector<int> jj; jj.push_back(ii);
			out = Block2->get_val(term.number, jj);
		}
		else
			out = calc(term.terms[0],ii);
	}

	if ((term.nterms == 1) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calc(term.terms[0],ii);

		if (term.operators[0] == 1)
			out = -calc(term.terms[0],ii);
	}

	if ((term.nterms == 2) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calc(term.terms[0],ii)+calc(term.terms[1],ii);
		if (term.operators[0] == 1)
			out = calc(term.terms[0],ii)-calc(term.terms[1],ii);
		if (term.operators[0] == 2)
			out = calc(term.terms[0],ii)*calc(term.terms[1],ii);
		if (term.operators[0] == 3)
		{
			double a = calc(term.terms[0], ii);
			double b = calc(term.terms[1], ii);
			if ((a == 0) && (b == 0))
				out = 0;
			else
				out = a / b;

		}
		if (term.operators[0] == 4)
			out = pow(calc(term.terms[0],ii),calc(term.terms[1],ii));
	}

	if ((term.nterms == 2) && (term.nopts == 2))
	{
		double sum = 0;
		if (term.operators[0] == 0)
			sum = calc(term.terms[0],ii);
		else if (term.operators[0] == 1)
			sum = -calc(term.terms[0],ii);

		if (term.operators[1] == 0)
			out = sum+calc(term.terms[1],ii);
		if (term.operators[1] == 1)
			out = sum-calc(term.terms[1],ii);
		if (term.operators[1] == 2)
			out = sum*calc(term.terms[1],ii);
		if (term.operators[1] == 3)
			out = sum/calc(term.terms[1],ii);
		if (term.operators[1] == 4)
			out = pow(fabs(sum),calc(term.terms[1],ii))*fabs(sum)/sum;


	}

	if ((term.nterms>2) && (term.nopts == term.nterms-1))
	{
		out = calc(term.terms[0],ii);
		for (int j=1; j<term.nterms; j++)
		{
			if (term.operators[j-1] == 0)
				out+=calc(term.terms[j],ii);
			if (term.operators[j-1] == 1)
				out-=calc(term.terms[j],ii);
			if (term.operators[j-1] == 2)
				out*=calc(term.terms[j],ii);
			if (term.operators[j - 1] == 3)
			{
				double a = calc(term.terms[j], ii);
				if ((out == 0) && (a == 0))
					out = 0;
				else
					out /= a;
			}
			if (term.operators[j-1] == 4)
				out=pow(out,calc(term.terms[j],ii));
		}

	}


	if ((term.nterms>2) && (term.nopts == term.nterms))
	{
		out = 0;
		if (term.operators[0] == 0)
			out = calc(term.terms[0],ii);
		else if (term.operators[0] == 1)
			out = -calc(term.terms[0],ii);


		for (int j=1; j<term.nterms; j++)
		{
			if (term.operators[j] == 0)
				out+=calc(term.terms[j],ii);
			if (term.operators[j] == 1)
				out-=calc(term.terms[j],ii);
			if (term.operators[j] == 2)
				out*=calc(term.terms[j],ii);
			if (term.operators[j] == 3)
				out/=calc(term.terms[j],ii);
			if (term.operators[j] == 4)
				out=pow(out,calc(term.terms[j],ii));
		}

	}

	if (term.function==true)
	{	if (term.number == exp_)
			return exp(out);
		if (term.number == hsd_)
			return Heavyside(out);
		if (term.number == lne_)
			return log(out);
		if (term.number == lnt_)
			return log10(out);
		if (term.number == sgm_)
			return 1.0/(1.0+exp(-out));
		if (term.number == pos_)
			return 0.5*(fabs(out)+out);
		if (term.number == sqr_)
			return sqrt(out);
		if (term.number == ply_)
			return pipe_poly(out);
		if (term.number==abs_)
			return fabs(out);
		if (term.number==sqs_)
		{
			if (out!=0)
				return out/fabs(out)*sqrt(fabs(out));
			else
				return 0;
		}

	}

	return out;
}

double CConnection::calc_star(const CStringOP &term, int ii)
{
	if (term.function==true)
    {
        if (term.number == min_)
			return min(calc_star(term.terms[0],ii), calc_star(term.terms[1],ii));
		if (term.number == max_)
			return max(calc_star(term.terms[0],ii), calc_star(term.terms[1],ii));
		if (term.number == sq1_)
			return 0.5/calc_star(term.terms[1],ii)*(calc_star(term.terms[0],ii)*calc_star(term.terms[1],ii)+sqrt(pow(calc_star(term.terms[0],ii)*calc_star(term.terms[1],ii),2)+1));
        if (term.number == frs_)
			return funcs[0].evaluate(get_val_star(9));
		if (term.number == fas_)
			return funcs[0].evaluate(get_val_star(4));
		if (term.number==mon_)
			return mon(calc_star(term.terms[0],ii),calc_star(term.terms[1],ii));
		if (term.number == sq2_)
		{	double term1 = calc_star(term.terms[0],ii);
			int sign_ = sgn(term1);
			double term2 = calc_star(term.terms[1],ii);
			return double(sign_)*pow(fabs(term1),(0.5*term1+term2)/(term1+term2));
		}
		if (term.number == mo1_)
			return mon(calc_star(term.terms[0], ii), calc_star(term.terms[1], ii))*calc_star(term.terms[0], ii) + mon(-calc_star(term.terms[0], ii), calc_star(term.terms[1], ii))*calc_star(term.terms[0], ii);
    }

	double out = 0;
	if ((term.nterms == 1) && (term.nopts == 0))
	{
		if (term.parameter == true)
			out = 0.5*(Block1->rxn_params[term.number-2000]+Block2->rxn_params[term.number-2000]);
		else if (term.concentration == true)
			if (Q>0) out = Block1->CG_star[term.number][term.phase]; else out = Block2->CG_star[term.number][term.phase];
		else if (term.s_concentration == true)
			if (term.number == -1)
				if (Q>0) out = Block1->G_star[ii][term.phase]; else out = Block2->G_star[ii][term.phase];
			else
				if (Q>0) out = Block1->G_star[term.number][term.phase]; else out = Block2->G_star[term.number][term.phase];
		else if (term.physical_ch == true)
			out = get_val_star(term.number,ii);
		else if (term.constant == true)
			out = term.value;
		else if (term.s_block == true)
		{
			vector<int> jj;
			jj.push_back(ii);
			out = Block1->get_val_star(term.number, jj);
		}
		else if (term.t_block == true)
		{
			vector<int> jj;
			jj.push_back(ii);
			out = Block2->get_val_star(term.number, jj);
		}
		else if (term.a_block == true)
		{
			vector<int> jj;
			jj.push_back(ii);
			out = 0.5*(Block1->get_val_star(term.number, jj) + Block1->get_val_star(term.number, jj));
		}
		else
		{
			out = calc_star(term.terms[0], ii);
		}
	}

	if ((term.nterms == 1) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calc_star(term.terms[0],ii);

		if (term.operators[0] == 1)
			out = -calc_star(term.terms[0],ii);
	}

	if ((term.nterms == 2) && (term.nopts == 1))
	{
		if (term.operators[0] == 0)
			out = calc_star(term.terms[0],ii)+calc_star(term.terms[1],ii);
		if (term.operators[0] == 1)
			out = calc_star(term.terms[0],ii)-calc_star(term.terms[1],ii);
		if (term.operators[0] == 2)
			out = calc_star(term.terms[0],ii)*calc_star(term.terms[1],ii);
		if (term.operators[0] == 3)
		{
			double a = calc_star(term.terms[0], ii);
			double b = calc_star(term.terms[1], ii);
			if ((a == 0) && (b == 0))
				out = 0;
			else
				out = a / b;

		}
		if (term.operators[0] == 4)
			out = pow(calc_star(term.terms[0],ii),calc_star(term.terms[1],ii));

	}

	if ((term.nterms == 2) && (term.nopts == 2))
	{
		double sum = 0;
		if (term.operators[0] == 0)
			sum = calc_star(term.terms[0],ii);
		else if (term.operators[0] == 1)
			sum = -calc_star(term.terms[0],ii);

		if (term.operators[1] == 0)
			out = sum+calc_star(term.terms[1],ii);
		if (term.operators[1] == 1)
			out = sum-calc_star(term.terms[1],ii);
		if (term.operators[1] == 2)
			out = sum*calc_star(term.terms[1],ii);
		if (term.operators[1] == 3)
			out = sum/calc_star(term.terms[1],ii);
		if (term.operators[1] == 4)
			out = pow(fabs(sum),calc_star(term.terms[1],ii))*fabs(sum)/sum;

	}

	if ((term.nterms>2) && (term.nopts == term.nterms-1))
	{
		out = calc_star(term.terms[0],ii);
		for (int j = 1; j < term.nterms; j++)
		{
			if (term.operators[j - 1] == 0)
				out += calc_star(term.terms[j], ii);
			if (term.operators[j - 1] == 1)
				out -= calc_star(term.terms[j], ii);
			if (term.operators[j - 1] == 2)
				out *= calc_star(term.terms[j], ii);
			if (term.operators[j - 1] == 3)
				out /= calc_star(term.terms[j], ii);
			if (term.operators[j - 1] == 4)
			{
				double a = calc_star(term.terms[j], ii);
				if ((out == 0) && (a == 0))
					out = 0;
				else
					out /= a;
			}
		}
	}


	if ((term.nterms>2) && (term.nopts == term.nterms))
	{
		out = 0;
		if (term.operators[0] == 0)
			out = calc_star(term.terms[0],ii);
		else if (term.operators[0] == 1)
			out = -calc_star(term.terms[0],ii);


		for (int j=1; j<term.nterms; j++)
		{
			if (term.operators[j] == 0)
				out+=calc_star(term.terms[j],ii);
			if (term.operators[j] == 1)
				out-=calc_star(term.terms[j],ii);
			if (term.operators[j] == 2)
				out*=calc_star(term.terms[j],ii);
			if (term.operators[j] == 3)
				out/=calc_star(term.terms[j],ii);
			if (term.operators[j] == 4)
				out=pow(out,calc_star(term.terms[j],ii));

		}

	}
	if (term.function==true)
	{	if (term.number == exp_)
			return exp(out);
		if (term.number == hsd_)
			return Heavyside(out);
		if (term.number == lne_)
			return log(out);
		if (term.number == lnt_)
			return log10(out);
		if (term.number == sgm_)
			return 1.0/(1.0+exp(-out));
		if (term.number == pos_)
			return 0.5*(fabs(out)+out);
		if (term.number == sqr_)
			return sqrt(out);
		if (term.number == ply_)
			return pipe_poly(out);
		if (term.number==abs_)
			return fabs(out);
		if (term.number == sqs_)
		{
			if (out != 0)
				return out / fabs(out)*sqrt(fabs(out));
			else
				return 0;
		}


	}

	return out;
}

double CConnection::get_val(int i, int ii)
{
	/* variable codes:
	H: 1
	A: 2
	V: 3
	S: 4
	z0: 5
	d: 6
	Q: 7
	v: 8
	Hydraulic Parameters: 50-99
	G: 101-199
	GS: 1000-1999
	Reaction parameters: 2000-2999
	*/

    if (i==state_vars::Head)
	{	if (Q>0)
			return Block1->H;
		else
			return Block2->H;
	}
    if (i==state_vars::Area) return A;
    if (i==state_vars::Volume)
	{	if (Q>0)
		return Block1->V;
	else
		return Block2->V;
	}
    if (i==state_vars::Storage)
	{	if (Q>0)
		return Block1->S;
	else
		return Block2->S;
	}
    if (i==state_vars::Bottom_Elev)
	{	if (Q>0)
		return Block1->z0;
	else
		return Block2->z0;
	}
    if (i==state_vars::Length) return d;
    if (i==state_vars::Flow_rate) return Q;
    if (i==state_vars::Liquid_flow) return (Q-Q_v)/A*flow_factor;

    if (i==state_vars::Effective_Moisture)
	{
		if (Block1->indicator==Soil || Block1->indicator == Plant || Block1->indicator == Darcy || Block1->indicator == Storage)
		{
			if (Block2->indicator==Soil || Block2->indicator == Plant || Block2->indicator == Darcy || Block2->indicator == Storage)
			{
				vector<int> jj;
				jj.push_back(ii);
                return max(Block1->get_val(state_vars::Effective_Moisture,jj),Block2->get_val(state_vars::Effective_Moisture,jj));
			}
			else
			{
				vector<int> jj;
				jj.push_back(ii);
                return Block1->get_val(state_vars::Effective_Moisture,jj);
			}
		}
		else
		{
			if (Block2->indicator==Soil || Block2->indicator == Plant)
			{
				vector<int> jj;
				jj.push_back(ii);
                return Block2->get_val(state_vars::Effective_Moisture,jj);
			}
			else
			{
				return 0;   //error output should be displayed here
			}
		}
	}
	//if (i==11) return Q*flow_factor;
    if (i==state_vars::Vapor_flow) return Q_v;
    if (i==state_vars::Dispersivity) return dispersivity;

	if ((i>=50) && (i<100)) return flow_params[i-50];

	if (i>=100 && i<2000)
	{	if (Q>0)
		return Block1->get_val(i);
	else
		return Block2->get_val(i);
	}
	if (i>=3000 && i<4000) return Solid_phase[ii]->c_params[i-3000];
	if (i==4000) return c_dispersion[i-4000];
	if ((i>=5000) && (i<5100))
		return dispersion[i-5000];

    return 0;
}

double CConnection::get_val_star(int i, int ii)
{
	/* variable codes:
	H: 1
	A: 2
	V: 3
	S: 4
	z0: 5
	d: 6
	Q: 7
	v: 8
	Hydraulic Parameters: 50-99
	G: 101-199
	GS: 1000-1999
	Reaction parameters: 2000-2999
	*/

    if (i==state_vars::Head)
	{	if (Q>0)
			return Block1->H_star;
		else
			return Block2->H_star;
	}
    if (i==state_vars::Area) return A_star;
    if (i==state_vars::Volume)
	{	if (Q>0)
		return Block1->V;
	else
		return Block2->V;
	}
    if (i==state_vars::Storage)
	{	if (Q>0)
		return Block1->S_star;
	else
		return Block2->S_star;
	}
    if (i==state_vars::Bottom_Elev)
	{	if (Q>0)
		return Block1->z0;
	else
		return Block2->z0;
	}
    if (i==state_vars::Length) return d;
    if (i==state_vars::Flow_rate) return Q_star;
    if (i==state_vars::Liquid_flow) return (Q_star-Q_v_star)/A_star*flow_factor;

    if (i==state_vars::Effective_Moisture)
	{
		if ((Block1->indicator==Soil) || (Block1->indicator == Plant) || (Block1->indicator == Darcy) || (Block1->indicator == Storage))
		{
			if ((Block2->indicator==Soil) || (Block2->indicator == Plant) || (Block2->indicator == Darcy) || (Block2->indicator == Storage))
			{
				return max(Block1->get_val_star(9),Block2->get_val_star(9));
			}
			else
			{
				return Block1->get_val_star(9);
			}
		}
		else
		{
			if (Block2->indicator==Soil)
			{
				return Block2->get_val_star(9);
			}
			else
			{
				return 0;   //error output should be displayed here
			}
		}
	}


    if (i==state_vars::Vapor_flow) return Q_v_star;
    if (i==state_vars::Dispersivity) return dispersivity;

	if ((i>=50) && (i<100)) return flow_params[i-50];
	if (i>=100 && i<2000)
	{	if (Q_star>0)
		return Block1->get_val_star(i);
	else
		return Block2->get_val_star(i);
	}

	if (i>=3000 && i<4000) return Solid_phase[ii]->c_params[i-3000];
	if (i==4000) return c_dispersion_star[i-4000];
	if ((i>=5000) && (i<5100)) 	return dispersion_star[i-5000];

    return 0;
}

bool CConnection::set_val(const string &SS, const double &val)
{
	vector<string> s = split(SS,',');
	if (s.size()==1)
	{
		if (tolower(trim(s[0]))=="a" || tolower(trim(s[0])) == "area") {A = val; A_star = val; return true; }
		if (tolower(trim(s[0]))=="d") {d = val; return true; }
		if (tolower(trim(s[0]))=="q") {Q = val;return true; }
		if (tolower(trim(s[0]))=="vel") {v = val;return true; }

		if (tolower(trim(s[0]))=="a*") {A_star = val;return true; }
		if (tolower(trim(s[0]))=="q*") {Q_star = val;return true; }
		if (tolower(trim(s[0]))=="vel*") {v_star = val;return true; }

		if (tolower(trim(s[0]))=="width") {flow_params[width] = val;return true; }
		if (tolower(trim(s[0]))=="nmanning") {flow_params[n_manning] = val;return true; }
		if (tolower(trim(s[0]))=="flow_exponent") {flow_params[flow_exponent] = val;return true; }


		if (tolower(trim(s[0]))=="ks") {flow_params[ks] = val;return true; }
		if (tolower(trim(s[0]))=="theta_s") {flow_params[theta_s] = val;return true; }
		if (tolower(trim(s[0]))=="theta_r") {flow_params[theta_r] = val;return true; }
		if (tolower(trim(s[0]))=="vg_alpha") {flow_params[vg_alpha] = val;return true; }
		if (tolower(trim(s[0]))=="vg_n") {flow_params[vg_n] = val;return true; }
		if (tolower(trim(s[0]))=="vg_m") {flow_params[vg_m] = val;return true; }
		if (tolower(trim(s[0]))=="lambda") {flow_params[lambda] = val;return true; }

		if (tolower(trim(s[0]))=="z1") {flow_params[z1] = val;return true; }
		if (tolower(trim(s[0]))=="z2") {flow_params[z2] = val;return true; }
		if (tolower(trim(s[0])) == "dam")
		{
			flow_params[z1] = val;
			flow_params[z2] = val;
			return true;
		}
		if (tolower(trim(s[0])) == "rating_curve_coeff") {flow_params[rating_curve_coeff] = val;return true; }
		if (tolower(trim(s[0])) == "rating_curve_power") {flow_params[rating_curve_power] = val;return true; }
		if (tolower(trim(s[0])) == "rating_curve_datum")
		{
		    flow_params[rating_curve_datum] = val;
		    return true;
		}


		if (tolower(trim(s[0]))=="diameter")
		{
			flow_params[2] = val;
			A_star=atan(1.0)*4*pow(val/2,2);
			A=atan(1.0)*4*pow(val/2,2);
			return true;

		}
		if (tolower(trim(s[0]))=="pipe_c") {flow_params[pipe_c] = val;return true; }

		if (tolower(trim(s[0]))=="q_v*") {Q_v_star = val;return true; }
		if (tolower(trim(s[0]))=="q_v") {Q_v = val;return true; }
		if (tolower(trim(s[0]))=="dispersivity") {dispersivity = val;return true; }
	}
	return false;

}

bool CConnection::set_val(int i, const double &val)
{
	if (i==2) {A = val; return true; }
	if (i==6) {d = val; return true; }
	if (i==7) {Q = val;return true; }
	if (i==13) {Q_v = val;return true; }


	return false;
}

bool CConnection::set_val_star(int i, const double &val)
{
	if (i==2) {A_star = val; return true; }
	if (i==6) {d = val; return true; }
	if (i==7) {Q_star = val;return true; }
	if (i==13) {Q_v_star = val;return true; }


	return false;
}



void CConnection::get_funcs(CStringOP &term)  //Works w/o reference(&)
{
	if ((term.nterms == 1) && (term.nopts == 0))
	{
		if (term.terms.size()>0)
			get_funcs(term.terms[0]);
	}

	if ((term.nterms == 2) && (term.nopts == 1))
	{
		get_funcs(term.terms[0]);
		get_funcs(term.terms[1]);
	}

	if ((term.nterms == 2) && (term.nopts == 2))
	{
		get_funcs(term.terms[0]);
		get_funcs(term.terms[1]);

	}

	if ((term.nterms>2) && (term.nopts == term.nterms-1))
	{
		for (int j=0; j<term.nterms; j++)
			get_funcs(term.terms[j]);
	}


	if ((term.nterms>2) && (term.nopts == term.nterms))
	{
		for (int j=0; j<term.nterms; j++)
			get_funcs(term.terms[j]);

	}
	if (term.function==true)
	{
	    if (term.number == 11)
		{	CFunction XX;
			XX.Expression = term;
			XX.Expression.function=false;
			XX.var_id = 0;
			XX._min=-0.3;
			XX._max=1.3;
			funcs.push_back(XX);
		}
		else if (term.number==12)
		{	CFunction XX;
			XX.Expression = term;
			XX.Expression.function=false;
			XX.var_id = 1;
			XX._min = -0.3*max(Block1->V,Block2->V);
			XX._max = 1.3*max(Block1->V,Block2->V);
			funcs.push_back(XX);
		}
	}
}

void CConnection::evaluate_functions(int i) //i=0->small s; i=1->large S
{
	funcs[i].X.clear();
	funcs[i].X.structured = true;

	for (double x=funcs[i]._min; x<=funcs[i]._max; x+=(funcs[i]._max-funcs[i]._min)/double(funcs[i].n_steps))
	{	if (funcs[i].var_id==0)
		{	Block1->set_val_star(9,x);
			Block2->set_val_star(9,x);
		}
		if (funcs[i].var_id==1)
		{	Block1->set_val_star(4,x);
			Block2->set_val_star(4,x);
		}

		funcs[i].X.append(x,calc_star(funcs[i].Expression));
	}

	funcs[i].X.structured = true;
	funcs[i].X = funcs[i].X.MA_smooth(2);

}

void CConnection::evaluate_functions()
{
	for (unsigned int i=0; i<funcs.size(); i++) evaluate_functions(i);
}

void CConnection::evaluate_dispersion()
{
	for (unsigned int i=0; i<Solid_phase.size(); i++)
		c_dispersion[i] = calc(Solid_phase[i]->dispersion, i) + Solid_phase[i]->diffusion;

}

void CConnection::evaluate_dispersion_star()
{
	for (unsigned int i=0; i<Solid_phase.size(); i++)
		c_dispersion_star[i] = calc_star(Solid_phase[i]->dispersion, i) + Solid_phase[i]->diffusion;

}

void CConnection::evaluate_const_dispersion()
{
	for (unsigned int i=0; i<RXN->cons.size(); i++)
		dispersion[i] = calc(dispersion_expression)+RXN->cons[i].diffusion;

}

void CConnection::evaluate_const_dispersion_star()
{
	for (unsigned int i=0; i<RXN->cons.size(); i++)
		dispersion_star[i] = calc_star(dispersion_expression)+RXN->cons[i].diffusion;
}

double CConnection::get_val(const string &S)
{
	/* variable codes:
	H: 1
	A: 2
	V: 3
	S: 4
	z0: 5
	d: 6
	Q: 7
	v: 8
	Hydraulic Parameters: 50-99
	G: 101-199
	GS: 1000-1999
	Reaction parameters: 2000-2999
	*/

	if (tolower(S) == "h")
	{
		if (Q>0)
			return Block1->H;
		else
			return Block2->H;
	}
	if (tolower(S) == "a") return A;
	if (tolower(S) == "volume")
	{
		if (Q>0)
			return Block1->V;
		else
			return Block2->V;
	}
	if (tolower(S) == "s")
	{
		if (Q>0)
			return Block1->S;
		else
			return Block2->S;
	}
	if (tolower(S) == "z0")
	{
		if (Q>0)
			return Block1->z0;
		else
			return Block2->z0;
	}
	if (tolower(S) == "d") return d;
	if (tolower(S) == "q") return Q*flow_factor;;
	if (tolower(S) == "q_net") return (Q - Q_v) / A*flow_factor;

	if (tolower(S) == "se")
	{
		if (Block1->indicator == Soil)
		{
			if (Block2->indicator == Soil)
			{
				return 0.5*(Block1->get_val("se") + Block2->get_val("se"));
			}
			else
			{
				return Block1->get_val("se");
			}
		}
		else
		{
			if (Block2->indicator == Soil)
			{
				return Block2->get_val("se");
			}
			else
			{
				return 0;   //error output should be displayed here
			}
		}
	}

	if (tolower(S) == "qv") return Q_v;
	if (tolower(S) == "dispersivity") return dispersivity;

    return 0;
}

bool CConnection::set_properties(string s)
{
        bool success = true;
        vector<string> ss = split(s);
        for (unsigned int i=0; i<ss.size(); i++)
        {
            vector<string> prop = split(ss[i],'=');
            if (prop.size()!=2)
            {
                errors.push_back(string("Syntax error in '") + ss[i] + string("' in command ") + s);
                show_message(string("Syntax error in '") + ss[i] + string("' in command ") + s);
                success = false;
            }
            bool done = set_property(prop[0],prop[1]);
            if (!done)
            {
                success = false;
            }

        }

        return success;

}

bool CConnection::set_property(string s, double value)
{
    bool success = true;

    bool done = set_val(s,value);
    if (!done)
    {
        errors.push_back("Property " + s + " was not found");
        show_message("Property " + s + " was not found");
        success = false;
    }

    return success;
}

bool CConnection::set_property(string s, string value)
{
    bool success = true;
    if (tolower(trim(s))=="name") {ID = value; show_message("Property [" + s + "] was set to " + value); return success;}
    if (tolower(trim(s))=="type")
    {
        bool done = settype(value);
        if (!done) {success = false; show_message("Property [" + s + "] was set to " + value); return success;}
    }
    bool done = set_val(s,atof(value.c_str()));
    if (!done)
    {
        errors.push_back("Property " + s + " was not found. In " + s);
        show_message("Property " + s + " was not found. In " + s);
        success = false;
    }


    show_message("Property [" + s + "] was set to " + value);

    return success;
}

void CConnection::show_message(string s)
{
    if (show_messages())
    {
        cout << "Connector [" + ID + "]:" + s << endl;
    }
}

bool CConnection::show_messages()
{
    bool parent_showmessages=false;
    if (parent)
        parent_showmessages = parent->showmessages;
    if (showmessages || parent_showmessages)
        return true;
    else
        return false;
}

bool CConnection::settype(string s)
{
	return 0;
}

string CConnection::tostring()
{
    string s;
    s += "Connector [" + ID + "]:\n";
    s += "   Name = " + ID + "\n";
    s += "   Area = " + numbertostring(A) + "\n";
    s += "   Flow_expression = " + flow_expression.expression + "\n";
    s += "   Flow = " + numbertostring(Q) + "\n";
    s += "   Velocity = " + numbertostring(v) + "\n";
    s += "   Width =" + numbertostring(flow_params[width]) + "\n";
    s += "   Manning coefficient =" + numbertostring(flow_params[n_manning]) + "\n";
    s += "   flow_exponent = " + numbertostring(flow_params[flow_exponent]) + "\n";
    s += "   Hydraulic conductivity = " + numbertostring(flow_params[ks]) + "\n";
    s += "   Theta_s = " + numbertostring(flow_params[theta_s]) + "\n";
    s += "   Theta_r = " + numbertostring(flow_params[theta_r]) + "\n";
    s += "   VG_alpha = " + numbertostring(flow_params[vg_alpha]) + "\n";
    s += "   VG_n = " + numbertostring(flow_params[vg_n]) + "\n";
    s += "   VG_m = " + numbertostring(flow_params[vg_m]) + "\n";
    s += "   VG_lambda = " + numbertostring(flow_params[lambda]) + "\n";
    s += "   start elevation = " + numbertostring(flow_params[z1]) + "\n";
    s += "   end elevation = " + numbertostring(flow_params[z2]) + "\n";
    s += "   rating curve coefficient = " + numbertostring(flow_params[rating_curve_coeff]) + "\n";
    s += "   rating curve power = " + numbertostring(flow_params[rating_curve_power]) + "\n";
    s += "   rating curve datum = " + numbertostring(flow_params[rating_curve_datum]) + "\n";
    s += "   Hazen-Williams Coefficient = " + numbertostring(flow_params[pipe_c]) + "\n";
    s += "   Dispersivity = " + numbertostring(dispersivity) + "\n";

	return s;

}
