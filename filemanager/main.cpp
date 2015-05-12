#include <iostream>
#include <armadillo>

#define BIG_VAL 1e+10

using namespace std;
using namespace arma;

// Datatype with the topology definitions
typedef struct topology {
	unsigned int dc;		// Data capture frequency
	unsigned int n_iter;	// Number of interations
	unsigned int n;			// Number of robots
	unsigned int opt_link;	// Optimization actived
	unsigned int ref_pass;	// Reference pass actived

	arma::mat init_x;		// Initial positions array
	arma::mat init_v;		// Initial velocities array

	arma::vec r_max;		// Maxium range of communication
	arma::vec r_min;		// Minimum range of security

	arma::vec timeout;		// Timeout configurations

	arma::mat x_ref;		// Reference position
} topology;

// Search a character in a string and return the position for this
bool find_char(std::string input, char search, unsigned int& pos) {
	for(unsigned int i = 0; i < input.size(); i++) {
		if(input[i] == search) {
			pos = i;
			return true;
		}
	}

	return false;
}

// Search a character in a string
bool find_char(std::string input, char search) {
	for(unsigned int i = 0; i < input.size(); i++) {
		if(input[i] == search) {
			return true;
		}
	}

	return false;
}

// Verify if a string input contains a substring search and return the start pose and end pose of this
bool contains(std::string input, string search, unsigned int &init_pose, unsigned int &end_pose) {
	std::size_t found = input.find(search);

	if(found != std::string::npos) {
		init_pose = found;
		end_pose = init_pose + search.size();
		return true;
	}
	return false;
}

// Verify if a string input contains a substring search
bool contains(std::string input, std::string search) {
	std::size_t found = input.find(search);

	if(found != std::string::npos) return true;
	return false;
}

// Extract all float numbers on a string
vector<float> get_float(std::string input) {
	vector<float> out;
	unsigned int n = 1;

	// Alias of size_t
	std::string::size_type sz;

	// While string dont end
	while(input.size() > 0) {
		out.resize(n);
		out[n-1] = std::stof(input,&sz);
		input = input.substr(sz);
		n++;
	}

	return out;
}

// Get data from a input filestream
rowvec get_data(const string search_key, ifstream &file_input) {
	unsigned int pose0, pose1;
	rowvec out;
	string line = "";

	// Find the search key
	while(!contains(line,search_key) && !file_input.eof()) {
		getline(file_input, line);
	}

	// If search key was fouded
	if(contains(line,search_key,pose0,pose1)) {
		// Get all number on the line
		vector<float> num = get_float(line.substr(pose1));

		// Set size of armadillo array
		out.resize(num.size());

		// Store on armadillo array
		for(unsigned int i = 0; i < num.size(); i++) {
			out(i) = num[i];
		}
	}

	// Reset file_input
	if(file_input.eof()) {
		file_input.clear();
		file_input.seekg(0,ios::beg);

	// Return to previous line
	} else file_input.seekg(-line.size()-1,ios::cur);

	return out;
}

// From data file stream, fill a armadillo array
mat fill_array(const string search_key, ifstream &file_input) {
	mat out;
	unsigned int n_bot = 0, init_pose, end_pose;
	string line = "";

	// Find the search key
	while(!contains(line,search_key) && !file_input.eof()) {
		getline(file_input, line);
		cout << line << endl;
	}

	// If search key was founded
	if(contains(line,search_key)) {
		stringstream bot_key;

		// Get new line
		getline(file_input, line);

		while(!contains(line,"[") && !file_input.eof()) {
			// Generate the robot search key
			bot_key.str("");
			bot_key << n_bot << ":";

			// Verity if the robot search key is present on line
			if(contains(line,bot_key.str(),init_pose,end_pose)) {
				// Get all number on the line
				vector<float> num = get_float(line.substr(end_pose));

				// Set size of armadillo array
				out.resize(n_bot+1,num.size());

				// Store on armadillo array
				for(unsigned int i = 0; i < num.size(); i++) {
					out(n_bot,i) = num[i];
				}

				// Increment the robot number
				n_bot++;

				// Read new line
				getline(file_input,line);

			// When the robot dont have data
			} else if(line[0] > 47 && line[0] < 58) {
				// Set size of armadillo
				out.resize(n_bot+1,1);

				// Store BIG_VAL
				out(n_bot,0) = BIG_VAL;

				// Increment the robot number
				n_bot++;
			} else {
				// Read new line
				getline(file_input,line);
			}
		}
	}

	// Reset file_input
	if(file_input.eof()) {
		file_input.clear();
		file_input.seekg(0,ios::beg);

	// Return to previous line
	} else file_input.seekg(-line.size()-1,ios::cur);

	return out;
}

// Read configuration file and store its data
topology read_topology(string config_file) {
	ifstream file_input;
	topology topology_data;

	const string sim_config = "[simulation-configuration]";
	const string robot_pose = "[robot-positions]";
	const string robot_vel = "[robot-velocities]";
	const string robot_ranges = "[robot-ranges]";
	const string reference = "[reference]";
	const string timeout = "[timeout]";

	file_input.open(config_file.c_str());

	if(file_input.is_open()) {
		// Get the simulation configurations
		topology_data.n_iter = get_data("n_iter:",file_input).at(0);
		topology_data.dc = get_data("dc:",file_input).at(0);
		topology_data.opt_link = get_data("opt_link:",file_input).at(0);
		topology_data.ref_pass = get_data("ref_pass:",file_input).at(0);

		// Store the initial positions and velocities
		topology_data.init_x = fill_array(robot_pose,file_input);
		topology_data.init_v = fill_array(robot_vel,file_input);

		// Store the number of robots
		topology_data.n = topology_data.init_x.n_rows;

		// Store the ranges of communication and security
		mat out = fill_array(robot_ranges,file_input);
		topology_data.r_max = out.col(0);
		topology_data.r_min = out.col(1);

		// Store the reference
		topology_data.x_ref = fill_array(reference,file_input);

		// Store the timeout
		topology_data.timeout = fill_array(timeout,file_input);

		fill_array("bola:",file_input);
	}

	file_input.close();

	return topology_data;
}

int main(int argc, char **argv) {
	// Read topology configuration from a external file
	topology topology_config = read_topology(argv[1]);

	//cout << "n_iter:" << topology_config.n_iter << endl;
	//cout << "dc:" << topology_config.dc << endl;
	//cout << "opt_link:" << topology_config.opt_link << endl;
	//cout << "ref_pass:" << topology_config.ref_pass << endl;
	//cout << "pose:\n" << topology_config.init_x << endl;
	//cout << "vel:\n" << topology_config.init_v << endl;
	//cout << "r_max:" << topology_config.r_max << endl;
	//cout << "r_min:" << topology_config.r_min << endl;
	//cout << "reference:" << topology_config.x_ref << endl;
	//cout << "timeout:" << topology_config.timeout << endl;

	//std::string orbits ("61111.97 365.24 123139.2 0.23");

	//vector<float> out = get_float(orbits);

	//for(unsigned int i = 0; i < out.size(); i++) {
		//printf("n:%.2f\n",out[i]);
	//}

	//std::string::size_type sz;     // alias of size_t

	//float mars = std::stof (orbits,&sz);
	//float earth = std::stof (orbits.substr(sz));
	////float extra = stof(orbits.substr(sz));

	//while(orbits.size() > 0) {
		//printf("data:%.2f\n",std::stof(orbits,&sz));
		//orbits = orbits.substr(sz);
	//}

	//cout << "substring:" << orbits.substr(sz) << endl;
	//cout << "sz:" << sz<< endl;
	//cout << "mars:" << mars << endl;
	//printf("mars:%f\n",mars);
	//cout << "earth:" << earth << endl;
	//cout << "extra:" << extra << endl;

	return 0;
}
