#include "JunctionSeed.h"


size_t JunctionSeed::m_junc_count = 1;

SpliceWay::SpliceWay() : chrom_name_ptr(NULL), start(0), spliceway_vec_ptr(NULL)
{
}

SpliceWay::SpliceWay(string* cnp, size_t st, vector<pair<size_t, int> >* svp) : chrom_name_ptr(cnp), start(st), spliceway_vec_ptr(svp)
{
}

SpliceWayTrue::SpliceWayTrue() : start(0)
{
}

SpliceWayTrue::SpliceWayTrue(const SpliceWay& sw) : chrom_name(*(sw.chrom_name_ptr)), start(sw.start), spliceway_vec(*(sw.spliceway_vec_ptr))
{
}

SpliceWayTrue::SpliceWayTrue(string* cnp, size_t st, vector<pair<size_t, int> >* svp) : chrom_name(*cnp), start(st), spliceway_vec(*svp)
{
}


JunctionSeed::JunctionSeed(const string& juncname, unsigned int hits, char strand, unsigned short kinds, size_t max_prefix_len, size_t max_suffix_len, 
	size_t start_blockoffset, size_t end_blockoffset,
	double entropy, unsigned short flankcase, const string& flankstring, double intronlen, double lpq, unsigned short min_mismatch,
	unsigned short max_mismatch, double ave_mismatch, size_t start, size_t end, const string& chrom, unsigned int unique_count, unsigned int multi_count, 
	unsigned int paired_count, unsigned int left_paired_count, unsigned int right_paired_count, unsigned int paired_mutiple_count, unsigned int paired_unique_count,
	unsigned int single_count, unsigned short min_anchor_difference): 
	m_juncname(juncname), m_hits(hits), m_strand(strand), m_max_prefix_len(max_prefix_len), 
	m_max_suffix_len(max_suffix_len), m_start_blockoffset(start_blockoffset), m_end_blockoffset(end_blockoffset),
	m_entropy(entropy), m_flankcase(flankcase), m_flankstring(flankstring), m_intronlen(intronlen), m_lpq(lpq), m_min_mismatch(min_mismatch), 
	m_max_mismatch(max_mismatch), m_ave_mismatch(ave_mismatch), m_start(start), m_end(end), m_chrom(chrom), m_left_exon(0), m_right_exon(0), 
	m_three_prime_known(false), m_five_prime_known(false), m_junc_id(m_junc_count++),
	m_min_anchor_difference(min_anchor_difference), m_unique_count(unique_count), m_multi_count(multi_count), 
	m_paired_count(paired_count), m_left_paired_count(left_paired_count), m_right_paired_count(right_paired_count),
	m_paired_mutiple_count(paired_mutiple_count), m_paired_unique_count(paired_unique_count),
	m_single_count(single_count), m_pair_known(false), m_filtered_type(NOT_FILTERED), m_encompass_reads_count(0), m_is_fusion(false)
{
	//if (chrom.find_first_of("chr"
}

JunctionSeed::JunctionSeed(size_t loc, size_t suffix_len, size_t rw, size_t tagidx, unsigned short mis, size_t strand, size_t strand2, size_t start, size_t end, 
	const string& chrom, const string& chrom2, size_t sam_count, const string& mate_match, int mate_diff, const vector<SpliceWay>& l_splice_ways, const vector<SpliceWay>& r_splice_ways, bool is_fusion, string insert) : 
	m_prefix_count(rw - 1, 0),  m_positive_count(0),  m_negative_count(0),  m_entropy(0), m_hits(0), m_flankcase(0), m_intronlen(0), m_left_exon(0), m_right_exon(0), 
	m_three_prime_known(false), m_five_prime_known(false), m_min_anchor_difference(-1), m_unique_count(0), m_multi_count(0), 
	m_paired_count(0), m_left_paired_count(0), m_right_paired_count(0), m_paired_mutiple_count(0), m_paired_unique_count(0), m_single_count(0), 
	m_filtered_type(NOT_FILTERED), m_pair_known(false), m_junc_id(m_junc_count++), m_max_min_prefix(0), m_max_min_suffix(0), m_is_fusion(is_fusion), m_encompass_reads_count(0)
{

	//cout <<111<<endl;

	//cout << "loc:" << loc << endl;

	//cout << "suffix_len:" << suffix_len << endl;

	if (loc < m_prefix_count.size())
		++ m_prefix_count[loc-1];
	else if (suffix_len < m_prefix_count.size())
		++ m_prefix_count[suffix_len-1];
	else
	{
		cerr << "anchor too long? "<< endl;
		cerr <<  loc << '\t' << suffix_len << endl;
	}	

	m_max_prefix_len = loc;

	m_max_suffix_len = suffix_len;

	m_max_mismatch = mis;

	m_min_mismatch = mis;

	m_sum_mismatch = mis;

	m_mapped_idx[tagidx] = 1;

	m_start = start;
	
	m_end = end;
	
	m_chrom = chrom;

	m_chrom2 = chrom2;

	if (m_is_fusion)
	{
		//cout <<222<<endl;

		vector<SpliceWay>::const_iterator v_iter;

		for (v_iter = l_splice_ways.begin(); v_iter != l_splice_ways.end(); ++v_iter)
		{
			//SpliceWayTure spt(
			//cout <<1111<<endl;
			if (v_iter == l_splice_ways.begin())
				left_splice_ways.push_back(SpliceWayTrue(*v_iter));
			else
			{
				left_splice_ways.back().spliceway_vec.push_back(make_pair(0,0));

				for (size_t i = 0; i < v_iter->spliceway_vec_ptr->size(); ++i)
					left_splice_ways.back().spliceway_vec.push_back((*v_iter->spliceway_vec_ptr)[i]);
			}
		}

		//cout <<333<<endl;

		for (v_iter = r_splice_ways.begin(); v_iter != r_splice_ways.end(); ++v_iter)
		{
			//cout <<2222<<endl;
			if (v_iter == r_splice_ways.begin())
				right_splice_ways.push_back(SpliceWayTrue(*v_iter));
			else
			{
				right_splice_ways.back().spliceway_vec.push_back(make_pair(0,0));

				for (size_t i = 0; i < v_iter->spliceway_vec_ptr->size(); ++i)
					right_splice_ways.back().spliceway_vec.push_back((*v_iter->spliceway_vec_ptr)[i]);
			}
		}
	}

	//cout <<444<<endl;

	if (strand & IS_REVERSE)
		m_strand1 = '-';
	else
		m_strand1 = '+';

	if (strand2 & IS_REVERSE)
		m_strand2 = '-';
	else
		m_strand2 = '+';

	if (insert == "")
		;
	else
		m_ins[insert] = 1;

	if (strand & IS_REVERSE)
		++ m_negative_count;
	else
		++ m_positive_count;

	//cout <<555<<endl;

	if (suffix_len > loc)
	{
		if (m_max_min_prefix < loc)
			m_max_min_prefix = static_cast<unsigned short> (loc);

		m_min_anchor_difference = static_cast<unsigned short> (suffix_len - loc);
	}
	else
	{
		if (m_max_min_suffix < suffix_len)
			m_max_min_suffix = static_cast<unsigned int> (suffix_len);

		if (suffix_len == loc)
		{
			if (m_max_min_prefix < loc)
				m_max_min_prefix = static_cast<unsigned short> (loc);
		}

		m_min_anchor_difference = static_cast<unsigned short> (loc - suffix_len);
	}

	//cout <<666<<endl;

	if (sam_count > 1)
		++m_multi_count;
	else
		++m_unique_count;

	//cout <<777<<endl;

	//cout << mate_match << endl;

	if (mate_match != "*")
	{
		++m_paired_count;

		if (mate_diff > 0)
			++m_left_paired_count;
		else if (mate_diff < 0)
			++m_right_paired_count;
		else
			cout <<"what?\t"<<mate_diff<<endl;

		if (sam_count > 1)
			++m_paired_mutiple_count;
		else
			++m_paired_unique_count;
	}			
	else
		++m_single_count;

	//cout <<888<<endl;
}

bool JunctionSeed::inc_hits(size_t idx, size_t suffix_len, size_t tagidx, unsigned short mis, size_t strand, size_t sam_count, const string& mate_match, int mate_diff, 
	const vector<SpliceWay>& l_splice_ways, const vector<SpliceWay>& r_splice_ways, string insert)
{
	if (tagidx != -1 && m_mapped_idx.find(tagidx) != m_mapped_idx.end())
		return false;

	if (insert == "")
		;
	else if (m_ins.find(insert) == m_ins.end())
		m_ins[insert] = 1;
	else
		++m_ins[insert];			

	if (idx < m_prefix_count.size())
		++ m_prefix_count[idx-1];
	else if (suffix_len < m_prefix_count.size())
		++ m_prefix_count[suffix_len-1];
	else
	{
		cerr << "anchor too long? "<< endl;
		cerr <<  idx << '\t' << suffix_len << endl;
	}

	m_mapped_idx[tagidx] = 1;

	if (m_max_prefix_len < idx)
		m_max_prefix_len = idx;

	if (m_max_suffix_len < suffix_len)
		m_max_suffix_len = static_cast<unsigned int> (suffix_len);

	if (mis > m_max_mismatch)
		m_max_mismatch = mis;

	if (mis < m_min_mismatch)
		m_min_mismatch = mis;

	m_sum_mismatch += mis;

	if (m_is_fusion)
	{
		vector<SpliceWay>::const_iterator v_iter;

		//for (v_iter = l_splice_ways.begin(); v_iter != l_splice_ways.end(); ++v_iter)
		//{
		//	//SpliceWayTure spt(
		//	left_splice_ways.push_back(SpliceWayTrue(*v_iter));
		//}

		//for (v_iter = r_splice_ways.begin(); v_iter != r_splice_ways.end(); ++v_iter)
		//{
		//	right_splice_ways.push_back(SpliceWayTrue(*v_iter));
		//}

		for (v_iter = l_splice_ways.begin(); v_iter != l_splice_ways.end(); ++v_iter)
		{
			//SpliceWayTure spt(
			if (v_iter == l_splice_ways.begin())
				left_splice_ways.push_back(SpliceWayTrue(*v_iter));
			else
			{
				left_splice_ways.back().spliceway_vec.push_back(make_pair(0,0));

				for (size_t i = 0; i < v_iter->spliceway_vec_ptr->size(); ++i)
					left_splice_ways.back().spliceway_vec.push_back((*v_iter->spliceway_vec_ptr)[i]);
			}
		}

		for (v_iter = r_splice_ways.begin(); v_iter != r_splice_ways.end(); ++v_iter)
		{
			if (v_iter == r_splice_ways.begin())
				right_splice_ways.push_back(SpliceWayTrue(*v_iter));
			else
			{
				right_splice_ways.back().spliceway_vec.push_back(make_pair(0,0));

				for (size_t i = 0; i < v_iter->spliceway_vec_ptr->size(); ++i)
					right_splice_ways.back().spliceway_vec.push_back((*v_iter->spliceway_vec_ptr)[i]);
			}
		}
	}

	if (strand & IS_REVERSE)
		++ m_negative_count;
	else
		++ m_positive_count;

	unsigned short min_anchor_difference;

	if (suffix_len > idx)
	{
		if (m_max_min_prefix < idx)
			m_max_min_prefix = static_cast<unsigned short> (idx);

		min_anchor_difference = static_cast<unsigned short> (suffix_len - idx);
	}
	else
	{
		if (m_max_min_suffix < suffix_len)
			m_max_min_suffix = static_cast<unsigned int> (suffix_len);

		if (suffix_len == idx)
		{
			if (m_max_min_prefix < idx)
				m_max_min_prefix = static_cast<unsigned short> (idx);
		}

		min_anchor_difference = static_cast<unsigned short> (idx - suffix_len);
	}

	if (min_anchor_difference < m_min_anchor_difference)
		m_min_anchor_difference = min_anchor_difference;

	if (sam_count > 1)
		++m_multi_count;
	else
		++m_unique_count;

	if (mate_match != "*")
	{
		++m_paired_count;

		if (mate_diff > 0)
			++m_left_paired_count;
		else if (mate_diff < 0)
			++m_right_paired_count;
		else
			cout <<"what?\t"<<mate_diff<<endl;

		if (sam_count > 1)
			++m_paired_mutiple_count;
		else
			++m_paired_unique_count;
	}
	else
		++m_single_count;

	return true;
}

size_t JunctionSeed::spliceways2exons_doner()
{
	//doner

	if (!m_is_fusion)
		return 0;

	size_t left_most = -1, right_most = 0;

	if (m_strand1 == '+')
	{
		//right_most = m_start;

		for (size_t i = 0; i < left_splice_ways.size(); ++i)
		{
			if (right_most < left_splice_ways[i].spliceway_vec.back().first + left_splice_ways[i].spliceway_vec.back().second)
				right_most = left_splice_ways[i].spliceway_vec.back().first + left_splice_ways[i].spliceway_vec.back().second;
		}

		for (size_t i = 0; i < left_splice_ways.size(); ++i)
		{
			if (left_most > left_splice_ways[i].start)
				left_most = left_splice_ways[i].start;
		}
	}
	else if (m_strand1 == '-')
	{
		//left_most = m_start;

		for (size_t i = 0; i < left_splice_ways.size(); ++i)
		{
			if (left_most > left_splice_ways[i].start)
				left_most = left_splice_ways[i].start;
		}

		for (size_t i = 0; i < left_splice_ways.size(); ++i)
		{
			if (right_most < left_splice_ways[i].spliceway_vec.back().first + left_splice_ways[i].spliceway_vec.back().second)
				right_most = left_splice_ways[i].spliceway_vec.back().first + left_splice_ways[i].spliceway_vec.back().second;
		}
	}

	vector<bool> exon_regions(right_most - left_most + 10, false);

	for (size_t i = 0; i < left_splice_ways.size(); ++i)
	{
		for (size_t j = 0; j < left_splice_ways[i].spliceway_vec.size(); ++j)
		{
			for (size_t k = left_splice_ways[i].spliceway_vec[j].first; k < left_splice_ways[i].spliceway_vec[j].first + left_splice_ways[i].spliceway_vec[j].second; ++k)
			{
				exon_regions[k - left_most] = true;
			}
		}
	}

	return hits2exons(left_exons, exon_regions, left_most);
}

size_t JunctionSeed::spliceways2exons_acceptor()
{
	//doner

	if (!m_is_fusion)
		return 0;

	size_t left_most = -1, right_most = 0;

	if (m_strand2 == '-')
	{
		for (size_t i = 0; i < right_splice_ways.size(); ++i)
		{
			if (right_most < right_splice_ways[i].spliceway_vec.back().first + right_splice_ways[i].spliceway_vec.back().second)
				right_most = right_splice_ways[i].spliceway_vec.back().first + right_splice_ways[i].spliceway_vec.back().second;
		}

		for (size_t i = 0; i < right_splice_ways.size(); ++i)
		{
			if (left_most > right_splice_ways[i].start)
				left_most = right_splice_ways[i].start;
		}
	}
	else if (m_strand2 == '+')
	{
		for (size_t i = 0; i < right_splice_ways.size(); ++i)
		{
			if (left_most > right_splice_ways[i].start)
				left_most = right_splice_ways[i].start;
		}

		for (size_t i = 0; i < right_splice_ways.size(); ++i)
		{
			if (right_most < right_splice_ways[i].spliceway_vec.back().first + right_splice_ways[i].spliceway_vec.back().second)
				right_most = right_splice_ways[i].spliceway_vec.back().first + right_splice_ways[i].spliceway_vec.back().second;
		}
	}

	vector<bool> exon_regions(right_most - left_most + 10, false);

	for (size_t i = 0; i < right_splice_ways.size(); ++i)
	{
		for (size_t j = 0; j < right_splice_ways[i].spliceway_vec.size(); ++j)
		{
			for (size_t k = right_splice_ways[i].spliceway_vec[j].first; k < right_splice_ways[i].spliceway_vec[j].first + right_splice_ways[i].spliceway_vec[j].second; ++k)
			{
				exon_regions[k - left_most] = true;
			}
		}
	}

	return hits2exons(right_exons, exon_regions, left_most);
}

size_t JunctionSeed::hits2exons(vector<pair<size_t, size_t> >& exons, vector<bool>& exon_regions, size_t left_most)
{
	bool inisland = false;

	size_t tmpstart = 0, tmpend = 0;

	for (size_t i = 0; i < exon_regions.size(); ++i)
	{
		if (exon_regions[i] && !inisland)
		{
			inisland = true;
			tmpstart = i;
		}
		else if(!exon_regions[i] && inisland)
		{
			tmpend = i;

			inisland = false;

			if (exons.size() && (tmpstart - exons[exons.size() - 1].second) <= boundary)
				exons[exons.size() - 1].second = tmpend - 1/* + 45*/;
			else
			{
				//if ((int)tmpstart - 45 < 0)
				//	cur_islands.push_back(make_pair(1, tmpend + 45));
				//else
				exons.push_back(make_pair(tmpstart/* - 45*/, tmpend - 1/* + 45*/));
			}
		}
	}

	for (size_t i = 0; i < exons.size(); ++i)
	{
		exons[i].first += left_most;

		exons[i].second += left_most;
	}

	return exons.size();
}

size_t JunctionSeed::construct_graph(vector<pair<size_t, size_t> >& exons, vector<SpliceWayTrue>& splice_ways, vector<vector<int> >& graph, char strand)
{
	for (size_t i = 0; i < splice_ways.size(); ++i)
	{
		vector<pair<size_t, int> >& spliceway_vec = splice_ways[i].spliceway_vec;

		if (spliceway_vec.size() > 1)
		{
			for (size_t j = 1; j < spliceway_vec.size(); ++j)
			{
				size_t start, end;

				int junc_type;

				if (spliceway_vec[j].second == 0)
				{
					start = spliceway_vec[j-1].first + spliceway_vec[j-1].second - 1;

					end = spliceway_vec[j+1].first;

					junc_type = 2;

					++j;
				}
				else
				{
					start = spliceway_vec[j-1].first + spliceway_vec[j-1].second - 1;

					end = spliceway_vec[j].first;

					junc_type = 1;
				}

				pair<size_t, size_t> cur_junc_region_start = make_pair(start, start);

				pair<size_t, size_t> cur_junc_region_end = make_pair(end, end);

				size_t start_region_idx = -1, end_region_idx = -1;

				bool find_start_region = FindRegion(cur_junc_region_start, exons, start_region_idx);

				bool find_end_region = FindRegion(cur_junc_region_end, exons, end_region_idx);

				if (find_start_region && find_end_region && start_region_idx != end_region_idx)
				{
					if (start_region_idx < end_region_idx)
					{
						if (strand == '+')
						{
							if( graph[start_region_idx][end_region_idx] != 1)
								graph[start_region_idx][end_region_idx] = junc_type;
						}
						else
						{
							if (graph[end_region_idx][start_region_idx] != 1)
								graph[end_region_idx][start_region_idx] = junc_type;
						}
					}
					else
					{
						//graph[start_region_idx][end_region_idx] = 1;
						cout << "start larger than end:" << start << '\t' << end << endl;
					}
					//cur_disjointSet.Union(start_region_idx, end_region_idx);
				}
				else if (!find_start_region)
				{
					cout << "start exon not found: "<<start <<endl;
					continue;
				}
				else if (!find_end_region)
				{
					cout << "end exon not found: "<<end <<endl;
					continue;
				}
				else if (start_region_idx == end_region_idx)
				{
					cout << "start exon and end exon equal: "<<endl
						<<start <<endl <<end <<endl;
					continue;
				}
				else
				{
					cout << "any thing else?"<<endl;
					continue;
				}
			}
		}
	}

	return 0;
}

bool
JunctionSeed::FindRegion(pair<size_t, size_t>& cur_region, vector<pair<size_t, size_t> >& sorted_regions, size_t& find_region_idx)
{
	vector<pair<size_t, size_t> >::iterator express_region_iter;

	express_region_iter = lower_bound(sorted_regions.begin(), sorted_regions.end(), cur_region, compare_pair_region);

	if (express_region_iter != sorted_regions.begin())
		--express_region_iter;

	bool find_region = false;

	vector<pair<size_t, size_t> >::iterator cur_express_region_iter = express_region_iter;

	while (true)
	{
		if (cur_express_region_iter == sorted_regions.end() || cur_express_region_iter->first > cur_region.first)
		{
			break;
		}
		else if (cur_express_region_iter->first  <= cur_region.first + 3 && cur_express_region_iter->second + 3 >= cur_region.first)
		{
			find_region = true;
			break;
		}

		++cur_express_region_iter;
	}

	if (find_region)
		find_region_idx = cur_express_region_iter - sorted_regions.begin();

	return find_region;
}

void JunctionSeed::set_coverage()
{
	m_hits =  m_positive_count +  m_negative_count;
}

void JunctionSeed::set_entropy()
{
	for (size_t i = 0; i <  m_prefix_count.size(); ++i)
	{
		if (m_prefix_count[i] > 0)
		{
			double pi =  m_prefix_count[i] / (double)m_hits;
			m_entropy += pi * log(pi);
		}
	}

	if ( m_entropy != 0)
		m_entropy = - m_entropy;
}

void JunctionSeed::set_flankstring(const string& flankstring)
{
	//char strand = '+';

	m_flankstring = flankstring;

	if (m_flankstring == "ATAC")
	{
		m_flankcase = 1;
		m_strand = '+';
	}
	else if (m_flankstring == "CTAC")
	{
		m_flankcase = 6;
		m_strand = '-';
	}
	else if (m_flankstring == "CTGC")
	{
		m_flankcase = 3;
		m_strand = '-';
	}
	else if (m_flankstring == "GCAG")
	{
		m_flankcase = 4;
		m_strand = '+';
	}
	else if (m_flankstring == "GTAG")
	{
		m_flankcase = 5;
		m_strand = '+';
	}
	else if (m_flankstring == "GTAT")
	{
		m_flankcase = 2;
		m_strand = '-';
	}
	else
	{
		m_flankcase = 0;
		m_strand = '+';
	}
}

void JunctionSeed::set_pq_score(size_t intron_len, size_t chrom_size)
{
	double ppower = pow(0.25, double(m_max_prefix_len));

	double pNpower = pow(1.0 - ppower, (double)chrom_size);

	double qpower = pow(0.25, double(m_max_suffix_len));

	double pDpower = pow(1.0 - qpower, (double)intron_len);

	double lpq = 1.0 - (pNpower * pDpower);

	double ppower2 = pow(0.25, double(m_max_prefix_len));

	double pNpower2 = pow(1.0 - ppower2, (double)intron_len );

	double qpower2 = pow(0.25, double(m_max_suffix_len));

	double pDpower2 = pow(1.0 - qpower2, (double)chrom_size);

	double lpq2 = 1.0 - (pNpower2 * pDpower2);

	m_lpq = 1.0 - (lpq + lpq2) / 2;
}

void JunctionSeed::set_il_score(size_t junc_st, size_t junc_end)
{
	m_il_score = 1.0 - (((double) (junc_end - junc_st - 1 + 1)) / (double (200000 - 1 + 2)));
}

void JunctionSeed::set_ave_mis()
{
	m_ave_mismatch = (double)m_sum_mismatch / (double)m_hits;
}

void JunctionSeed::set_block_offset(size_t start, size_t end)
{
	m_start_blockoffset = 0;

	m_end_blockoffset = end + m_max_suffix_len - start + 1;
}

string JunctionSeed::to_normal_junction(size_t junc_id)
{
	char buf[5000];

	if (!m_is_fusion)
		sprintf(buf, "%s\t%llu\t%llu\tJUNC_%llu\t%u\t%c\t%llu\t%llu\t255,0,0\t2\t%llu,%llu,\t%llu,%llu,\t%lf\t%hu\t%s\t%lf\t%lf\t%hu\t%hu\t%lf\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%hu", m_chrom.c_str(), 
		m_start, m_end, junc_id, m_hits, m_strand, m_start, m_end, m_max_prefix_len, m_max_suffix_len, m_start_blockoffset, m_end_blockoffset, m_entropy, 
		m_flankcase, m_flankstring.c_str(), m_il_score, m_lpq, m_min_mismatch, m_max_mismatch, m_ave_mismatch, m_unique_count, m_multi_count, m_paired_count, 
		m_left_paired_count, m_right_paired_count, m_paired_mutiple_count, m_paired_unique_count, m_single_count, m_min_anchor_difference);
	else
	{
		if ( m_flankstring == "CTAC")
		{
			char strand1 = (m_strand2 == '+' ? '-' : '+');

			char strand2 = (m_strand1 == '+' ? '-' : '+');

			size_t fusion_doner_start, fusion_acceptor_end;

			if (strand1 == '+')
			{
				fusion_doner_start = m_end - m_max_suffix_len;
			}
			else
			{
				fusion_doner_start = m_end + m_max_suffix_len;
			}


			if (strand2 == '+')
			{
				fusion_acceptor_end = m_start + m_max_prefix_len;
			}
			else
			{
				fusion_acceptor_end = m_start - m_max_prefix_len;
			}

			sprintf(buf, "%s_%s\t%llu\t%llu\tJUNC_%llu\t%u\t%c%c\t255,0,0\t2\t%llu,%llu,\t%llu,%llu,\t%lf\t%hu\t%s\t%hu\t%hu\t%lf\t%hu\t%hu\t%hu\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u",
				m_chrom2.c_str(), m_chrom.c_str(), 
				m_end, m_start, junc_id, m_hits, strand1, strand2, m_max_suffix_len, m_max_prefix_len, m_start_blockoffset, m_end_blockoffset, m_entropy, 
				6, revcomp(m_flankstring).c_str(), m_min_mismatch, m_max_mismatch, m_ave_mismatch, m_max_min_suffix, m_max_min_prefix, m_min_anchor_difference, 
				m_unique_count, m_multi_count, 
				m_paired_count, m_left_paired_count, m_right_paired_count, m_paired_mutiple_count, m_paired_unique_count, 
				m_single_count, 
				m_encompass_reads_count, fusion_doner_start, fusion_acceptor_end);
		}
		else
		{
			size_t fusion_doner_start, fusion_acceptor_end;

			if (m_strand1 == '+')
			{
				fusion_doner_start = m_start - m_max_prefix_len; 
			}
			else
			{
				fusion_doner_start = m_start + m_max_prefix_len; 
			}

			if (m_strand2 == '+')
			{
				fusion_acceptor_end = m_end + m_max_suffix_len;
			}
			else
			{
				fusion_acceptor_end = m_end - m_max_suffix_len;
			}

			sprintf(buf, "%s_%s\t%llu\t%llu\tJUNC_%llu\t%u\t%c%c\t255,0,0\t2\t%llu,%llu,\t%llu,%llu,\t%lf\t%hu\t%s\t%hu\t%hu\t%lf\t%hu\t%hu\t%hu\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u", m_chrom.c_str(), m_chrom2.c_str(), 
			m_start, m_end, junc_id, m_hits, m_strand1, m_strand2, m_max_prefix_len, m_max_suffix_len, m_start_blockoffset, m_end_blockoffset, m_entropy, 
			m_flankcase, m_flankstring.c_str(), m_min_mismatch, m_max_mismatch, m_ave_mismatch, m_max_min_prefix, m_max_min_suffix, m_min_anchor_difference, 
			m_unique_count, m_multi_count, 
			m_paired_count, m_left_paired_count, m_right_paired_count, m_paired_mutiple_count, m_paired_unique_count, 
			m_single_count, 
			m_encompass_reads_count, fusion_doner_start, fusion_acceptor_end);
		}
	}
		//chr19_chr5	21558671	176048215	JUNC_42	6	-+	255,0,0	2	65,66	0,154489544	1.79176	6	CTAC	
		//AGGAAGCTGCTGAAGAACCACTGATTGAGCCCCTGATGGAGCCAGAAGGGGAGAGTTATGAGGACCCACCCCAGGTATTGATGTCTCTAAGCCAGATCTGATCACCTGTCTGGAGCAAGGAAAAGATCCCTGGAATATGAAGAGACAC	
		//1	2	1.33333	9	24	27

		

	return buf;

}

string JunctionSeed::to_normal_junction_bed(size_t junc_id)
{
	return "test";
}

string JunctionSeed::to_insert_junction(size_t junc_id)
{
	char buf[5000];

	string ins_str;

	map<string, int>::iterator ins_iter;
	for (ins_iter = m_ins.begin(); ins_iter != m_ins.end(); ++ins_iter)
	{
		ins_str.append(ins_iter->first);
		ins_str.append("-");
		char intbuf[10];

		sprintf(intbuf, "%d", ins_iter->second);
		ins_str.append(intbuf);
		ins_str.append(",");
	}

	sprintf(buf, "%s\t%llu\t%llu\tJUNC_%llu\t%u\t%c\t%llu\t%llu\t255,0,0\t2\t%llu,%llu,\t%llu,%llu,\t%lf\t%hu\t%s\t%lf\t%lf\t%hu\t%hu\t%lf\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%u\t%hu\t%s", m_chrom.c_str(), 
		m_start, m_end, junc_id, m_hits, m_strand, m_start, m_end, m_max_prefix_len, m_max_suffix_len, m_start_blockoffset, m_end_blockoffset, m_entropy, 
		m_flankcase, m_flankstring.c_str(), m_il_score, m_lpq, m_min_mismatch, m_max_mismatch, m_ave_mismatch, m_unique_count, m_multi_count, m_paired_count, 
		m_left_paired_count, m_right_paired_count, m_paired_mutiple_count, m_paired_unique_count, m_single_count, m_min_anchor_difference, ins_str.c_str());

	return buf;
}

FusionJuncRegion::FusionJuncRegion(JunctionSeed* junc_seed_ptr) : m_junc_seed_ptr(junc_seed_ptr)
{
	if (m_junc_seed_ptr->m_strand1 == '+')
	{
		m_doner_st = m_junc_seed_ptr->m_start - fusion_region;

		m_doner_end = m_junc_seed_ptr->m_start;
	}
	else if (m_junc_seed_ptr->m_strand1 == '-')
	{
		m_doner_st = m_junc_seed_ptr->m_start;

		m_doner_end = m_junc_seed_ptr->m_start + fusion_region;
	}

	if (m_junc_seed_ptr->m_strand2 == '+')
	{
		m_acceptor_st = m_junc_seed_ptr->m_end;

		m_acceptor_end = m_junc_seed_ptr->m_end + fusion_region;
	}
	else if (m_junc_seed_ptr->m_strand2 == '-')
	{
		m_acceptor_st = m_junc_seed_ptr->m_end - fusion_region;

		m_acceptor_end = m_junc_seed_ptr->m_end;
	}
}

FusionJuncRegion::FusionJuncRegion(size_t doner_st, size_t doner_end, size_t acceptor_st, size_t acceptor_end) : 
		m_doner_st(doner_st), m_doner_end(doner_end), m_acceptor_st(acceptor_st), m_acceptor_end(acceptor_end)
{
}

FusionJuncRegion::FusionJuncRegion()
{
}

void FusionJuncRegion::set(size_t doner_st, size_t doner_end, size_t acceptor_st, size_t acceptor_end)
{
	m_doner_st = doner_st;
	
	m_doner_end = doner_end;
	
	m_acceptor_st = acceptor_st;
	
	m_acceptor_end = acceptor_end;
}
