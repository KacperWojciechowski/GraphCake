#include "..\inc\Graph_list.h"

#include <iomanip>

/**
 * \brief Constructor creating the graph from file.
 * 
 * The constructor accepts the path to a file in supported format. If format is incorrect
 * or the file cannot be accessed, an exception is returned. 
 * 
 * \param file_path Path of the data source file.
 * \param name Name of the graph (user-given).
 * \param type Type of the graph (from the Graph::Type enum).
 * 
 * \warning Exceptions to guard against:
 *		- std::invalid_argument - when given file extension is not supported.
 *		- std::runtime_error	- when given file cannot be accessed.
 * \attention Supported formats are .lst and .GRAPHML. The .lst representation does not
 *			  contain weights, so a weight of 1 is assumed for each connection if using
 *			  this file format as a data source. 
 * \see House of Graphs for .lst adjacency list file reference.
 */
Graph::List::List(std::string file_path, std::string name, Type type)
	: name(name),
	type(type)
{
	// loading the .lst file
	if (file_path.find(".lst", 0) != std::string::npos)
	{
		std::fstream file(file_path, std::ios::in);
		if (file.good())
		{
			this->load_lst_file(file);
		}
		else
		{
			throw std::runtime_error("Could not open file");
		}
		file.close();

		this->calculate_degrees();
	}
	// loading the .GRAPHML file
	else if (file_path.find(".GRAPHML", 0) != std::string::npos)
	{
		std::fstream file(file_path, std::ios::in);
		if (file.good())
		{
			this->load_graphml_file(file);
		}
		else
		{
			throw std::runtime_error("Could not open file");
		}
		file.close();
		
		this->calculate_degrees();
	}
	// signalizing the unsupported file format
	else
	{
		throw std::invalid_argument("File not supported");
	}
}




/**
 * \brief Constructor creating the adjacency list object based on the adjacency matrix object.
 * 
 * \param matrix Reference to the Graph::Matrix object.
 */
Graph::List::List(Matrix& matrix)
{
	// get general graph info
	this->name = matrix.get_name();
	this->type = matrix.get_type();
	
	// build list based on the matrix
	for (std::size_t i = 0; i < matrix.get_nodes_amount(); i++)
	{
		this->list.push_back({});

		for (std::size_t j = 0; j < matrix.get_nodes_amount(); j++)
		{
			if (matrix.get_edge(i, j) != 0)
			{
				this->list[i].push_back({ j, matrix.get_edge(i, j) });
			}
		}
	}

	this->calculate_degrees();
}




/**
 * \brief Function loading the data from .lst file into the object.
 * 
 * This function is for internal use only. To create an object containig data
 * from .lst file, use the specified constructor.
 * 
 * \see List::List(std::string file_path, std::string name, Graph::Type type)
 * 
 * \param file Reference to the file object of the data source.
 * 
 */
void Graph::List::load_lst_file(std::fstream& file)
{
	std::string line;
	size_t pos;

	std::size_t amount = 0;

	// calculate the amount of lines in the file
	while (!file.eof())
	{
		std::getline(file, line);
		amount++;
	}
	
	file.clear();
	file.seekg(std::ios_base::beg);

	Node vertex;

	std::cout << amount << std::endl;

	// extract each of the adjacent vertices
	for (std::size_t i = 0; i < amount; i++)
	{
		std::getline(file, line);

		if (line != "")
		{
			this->list.push_back({});

			std::cout << this->list.size() << std::endl;

			pos = line.find(' ');

			auto itr = std::next(this->list.begin(), this->list.size() - 1);

			while (pos != std::string::npos)
			{
				line = line.substr(pos + 1);
				vertex.ID = static_cast<std::size_t>(atoi(line.c_str())) - 1;
				vertex.weight = 1;
				itr->push_back(vertex);
				pos = line.find(' ', pos);
			}
		}
	}
}




/**
 * \brief Function loading the data from .GRAPHML file.
 * 
 * \param file Reference to the file object of the data source.
 * 
 * \warning Exceptions to guard against:
 *		- std::runtime_error When weight is less or equal to 0.
 */
void Graph::List::load_graphml_file(std::fstream& file)
{
	std::string line;
	std::string line2;

	// set the default position of graph ID
	std::size_t pos = std::string::npos;
	std::size_t pos2 = std::string::npos;

	std::string weight_id;

	// search for graph ID and weight marker ID
	while (pos == std::string::npos)
	{
		std::getline(file, line);
		line2 = line;
		// search for the key marker
		pos = line.find("key");
		if (pos != std::string::npos)
		{
			// search for the weight keyword
			pos2 = line.find("weight");
			// if the keyword was not found, check the next line
			if (pos2 == std::string::npos)
			{
				std::getline(file, line2);
				pos2 = line2.find("weight");
			}
			// if the keyword was found, save the ID, else skip
			if (pos2 != std::string::npos)
			{
				weight_id = line.substr(pos + 8, 2);
			}
		}
		pos = line2.find("graph id=");
	}

	// search for the edge type
	pos2 = line.find("edgedefault=");

	// save ID
	this->name = line.substr(pos + 10, pos2 - 2 - (pos + 10));

	// search for the end of the marker and save graph type
	pos = line.find("\">");
	std::string type = line.substr(pos2 + 13, pos - (pos2 + 13));

	if (type == "undirected")
	{
		this->type = Type::undirected;
	}
	else if (type == "directed")
	{
		this->type = Type::directed;
	}
	else
	{
		this->type = Type::undefined;
	}

	// search for vertices count
	std::getline(file, line);
	pos = line.find("node");

	std::size_t vertices = 0;

	while (pos != std::string::npos)
	{
		vertices++;
		std::getline(file, line);
		pos = line.find("node");
	}

	// create empty adjacency list

	for (std::size_t i = 0; i < vertices; i++)
	{
		this->list.push_back({});
	}
	
	// find the edge marker
	pos = line.find("edge");

	std::string id1;
	std::string id2;

	int32_t weight;
	std::string weight_str;

	// extract edge info
	while (pos != std::string::npos)
	{
		// find and extract the source and target nodes IDs
		pos = line.find("source=");
		pos2 = line.find("\" target=");

		id1 = line.substr(pos + 9, pos2 - (pos + 9));
		pos = line.find("\">");
		id2 = line.substr(pos2 + 11, pos - (pos2 + 11));

		// if the weight is specified, look for it
		if (!weight_id.empty())
		{
			pos = line.find("data");
			pos2 = line.find(weight_id);
			// search this and next line for the right data tag
			do {
				std::getline(file, line);
				pos = line.find("data");
				pos2 = line.find(weight_id);
			} while (pos != std::string::npos && pos2 == std::string::npos);

			// if data tags ended, assume the weight as 1
			if (pos == std::string::npos)
			{
				weight = 1;
			}
			// else extract the right weight
			else
			{
				pos = line.find("<", pos2);
				weight = atoi(line.substr(pos2 + 4, pos - (pos2 + 4)).c_str());

				if (weight <= 0)
				{
					throw std::runtime_error("Weight less or equal to zero");
				}

				// skip the </edge> closing tag and load the next edge tag
				std::getline(file, line);
				std::getline(file, line);
			}
		}
		// if no weight data ID was given, assume all weights as 1
		else
		{
			weight = 1;
		}

		// input the connection into the list
		this->list[atoi(id1.c_str())].push_back({ static_cast<std::size_t>(atoi(id2.c_str())), static_cast<uint32_t>(weight) });

		// if graph is undirected, make the connection both ways
		if (this->type == Type::undirected)
		{
			this->list[atoi(id2.c_str())].push_back({ static_cast<std::size_t>(atoi(id1.c_str())), static_cast<uint32_t>(weight) });
		}

		// search for next edge marker
		pos = line.find("edge");
		if (pos == std::string::npos || line[pos - 1] == '/')
		{
			std::getline(file, line);
		}
		pos = line.find("edge");
	}
}




/**
 * \brief Function for calculating the degrees of each vertex.
 * 
 * This function is for internal use only.
 * 
 */
void Graph::List::calculate_degrees()
{
	// calculate the degree of each vertex
	for (std::size_t i = 0; i < this->list.size(); i++)
	{
		this->degrees.push_back({ 0, 0, 0 });
		for (auto itr = this->list[i].begin(); itr != this->list[i].end(); itr++)
		{
			this->degrees[i].out_deg++;

			if (itr->ID == i)
			{
				this->degrees[i].deg += 2;
			}
			else
			{
				this->degrees[i].deg++;
			}
		}
	}

	if (this->type == Type::undirected)
	{
		for (std::size_t i = 0; i < this->degrees.size(); i++)
		{
			this->degrees[i].in_deg = this->degrees[i].out_deg;
		}
	}
	else
	{
		for (std::size_t i = 0; i < this->degrees.size(); i++)
		{
			for (std::size_t j = 0; j < this->list.size(); j++)
			{
				for (auto itr = this->list[j].begin(); itr != this->list[j].end(); itr++)
				{
					if (itr->ID == i)
					{
						this->degrees[i].in_deg++;
						break;
					}
				}
			}
		}
	}
}




/**
 * \brief Simple copy constructor.
 * 
 * \param l lvalue reference to the data source object.
 */
Graph::List::List(List& l)
	: list(l.list),
	name(l.name),
	type(l.type),
	degrees(l.degrees)
{
}




/**
 * \brief Simple move constructor.
 * 
 * \param l rvalue reference to the data source object
 */
Graph::List::List(List&& l) noexcept
	: list(l.list),
	name(l.name),
	type(l.type),
	degrees(l.degrees)
{
	l.list.clear();
	l.name.clear();
	l.degrees.clear();
}




/**
 * \brief Function printing information regarding the graph.
 * 
 * Information displayed are such as:
 *  - Name of the graph
 *  - Type of the graph
 *	- ID of the vertex
 *	- IDs of adjacent vertexes
 *  - Weights of the connections
 *  - Degree of each vertex
 * 
 *	\note Data is displayed in format "degree: ____*, ID: adj_vert_ID {weight}, adj_vert_ID {weight}, ...",
 *		  but the degree field varies based on the graph type. In case of undirected graph, only one value is shown.
 *		  In case of directed graphs, indegree and outdegree is shown in format "in|out".
 * 
 */
void Graph::List::print()
{
	// display graph name and type info
	std::cout << "Name = " << this->name << std::endl;
	std::cout << "Type = ";

	switch (this->type)
	{
	case Type::undirected:
		std::cout << "undirected ";
		break;

	case Type::directed:
		std::cout << "directed ";
		break;

	default:
		std::cout << "undefined ";
		break;
	}

	std::cout << std::endl;

	// display vertices count
	std::cout << "Vertices = " << this->list.size() << std::endl;

	std::cout << "{" << std::endl;

	std::size_t index;

	for (auto itr = this->list.begin(); itr != this->list.end(); itr++)
	{
		index = std::distance(this->list.begin(), itr);

		// display degrees
		if (this->type == Type::undirected)
		{
			std::cout << "degree: " << std::left << std::setw(6) << this->degrees[index].deg;
		}
		else
		{
			std::cout << "degrees: (in|out) " << std::left << std::setw(4) << this->degrees[index].in_deg << " | " << this->degrees[index].out_deg;
		}

		std::cout << ", " << index << ": ";

		for (auto itr2 = itr->begin(); itr2 != itr->end(); itr2++)
		{
			std::cout << itr2->ID << " {" << itr2->weight << "}, ";
		}
		std::cout << std::endl;
	}

	std::cout << "}" << std::endl;
}




/**
 * \brief Function adding a connection between two given vertices.
 * 
 * \warning If an edge between given vertices already exists, the weight of the 
 *			connection will be overwritten, and second edge will not be added.
 * 
 * \param source ID of the source vertex.
 * \param destination ID of the end vertex.
 * \param weight Weight of the connection.
 */
void Graph::List::add_edge(std::size_t source, std::size_t destination, uint32_t weight)
{
	// validate arguments
	if (source >= this->list.size() || destination >= this->list.size())
	{
		throw std::out_of_range("Index out of bounds");
	}
	if (weight == 0)
	{
		throw std::invalid_argument("Weight equal to zero");
	}

	// search for the iterator of connection, if added edge already exists
	auto edge_itr = this->list[source].end();
	for (auto itr = this->list[source].begin(); itr != this->list[source].end(); itr++)
	{
		if (itr->ID == destination)
		{
			edge_itr = itr;
			break;
		}
	}

	// if iterator was found, override the weight of the connection
	if (edge_itr != this->list[source].end())
	{
		edge_itr->weight = weight;
	}
	// else insert the connection to the structure
	else
	{
		this->list[source].push_back({ destination, weight });
		
		// calculate degrees
		this->degrees[source].out_deg++;
		this->degrees[source].deg++;
		this->degrees[destination].in_deg++;
	}

	// if the graph type is undirected
	if (this->type == Type::undirected)
	{
		// search for the iterator of mirrored connection 
		edge_itr = this->list[destination].end();
		for (auto itr = this->list[destination].begin(); itr != this->list[destination].end(); itr++)
		{
			if (itr->ID == source)
			{
				edge_itr = itr;
				break;
			}
		}

		// if iterator was found, override the weight of the connection
		if (edge_itr != this->list[destination].end())
		{
			edge_itr->weight = weight;
		}
		// else insert the mirrored connection to the structure
		else
		{
			this->list[destination].push_back({ source, weight });
			this->degrees[destination].out_deg++;
			this->degrees[destination].deg++;
			this->degrees[source].in_deg++;
		}
	}
}




/**
 * \brief Function adding a new vertex to the graph structure.
 * 
 * Added vertex is initially isolated, so any connections to it need
 * to be added manually.
 * 
 */
void Graph::List::add_node()
{
	this->list.push_back({});
	this->degrees.push_back({ 0, 0, 0 });
}




/**
 * \brief Function removing a connection between two given adjacency vertices.
 * 
 * \param source ID of the beginning vertex.
 * \param destination ID of the end vertex.
 * 
 * \warning Exceptions to guard against:
 *		- std::out_of_range - When any of the given IDs is out of bounds for the adjacency list.
 */
void Graph::List::remove_edge(std::size_t source, std::size_t destination)
{
	// validate the parameter
	if (source >= this->list.size() || destination >= this->list.size())
	{
		throw std::out_of_range("Index out of bounds");
	}

	// erase the edge
	for (auto itr = this->list[source].begin(); itr != this->list[source].end(); itr++)
	{
		if (itr->ID == destination)
		{
			this->list[source].erase(itr);

			this->degrees[source].out_deg--;
			this->degrees[source].deg--;
			this->degrees[destination].in_deg--;
		}
	}

	// if the graph is undirected, remove the mirrored edge 
	if (this->type == Type::undirected)
	{
		for (auto itr = this->list[destination].begin(); itr != this->list[destination].end(); itr++)
		{
			if (itr->ID == source)
			{
				this->list[destination].erase(itr);

				this->degrees[destination].out_deg--;
				this->degrees[destination].deg--;
				this->degrees[source].in_deg--;
			}
		}
	}
}




/**
 * \brief Function for removing a vertex from the graph structure.
 * 
 * \param node_id ID of the vertex to be removed from the graph structure.
 * 
 * \warning Removal of a vertex causes the re-enumeration of each subsequent vertex,
 *			by decreasing their indexes by 1.
 * 
 * \warning Exception to guard against:
 *			- std::out_of_range - when given vertex ID is out of bounds for the list.
 */
void Graph::List::remove_node(std::size_t node_id)
{
	// validate parameter
	if (node_id >= this->list.size())
	{
		throw std::out_of_range("Index out of bounds");
	}

	// variables for vertex removal
	bool removed = false;
	std::list<Node>::iterator itr_tmp;
	std::size_t count = this->list.size();

	// search through the list
	for (std::size_t i = 0; i < count; i++)
	{
		// remove the part of the list where the given vertex is the source of an edge
		if (!removed && i == node_id)
		{
			removed = true;
			this->list.erase(std::next(this->list.begin(), i));
			this->degrees.erase(std::next(this->degrees.begin(), i));
			count = this->list.size();
			i--;
		}
		// remove the parts of the list where the given vertex is the end of an edge
		else
		{
			for (auto itr2 = this->list[i].begin(); itr2 != this->list[i].end();)
			{
				// if given vertex was found
				if (itr2->ID == node_id)
				{
					// erase the vertex
					itr_tmp = itr2;
					itr2++;
					this->list[i].erase(itr_tmp);
					this->degrees[i].deg--;
					this->degrees[i].in_deg--;
					continue;
				}
				// if vertex of higher ID than the given one was found
				else if (itr2->ID > node_id)
				{
					itr2->ID--;
					
				}
				// move to the next vertex
				itr2++;
			}
		}
	}
}




/**
 * \brief Getter for vertices amount.
 * 
 * \return Amount of vertices as const.
 */
const std::size_t Graph::List::get_nodes_amount()
{
	return this->list.size();
}




/**
 * \brief Getter for vertex degree.
 * 
 * \param node_id ID of the vertex which degree should be returned.
 * \return Degree of the vertex as const.
 */
const Graph::Degree Graph::List::get_node_degree(std::size_t node_id)
{
	// validate the parameter
	if (node_id >= this->degrees.size())
	{
		throw std::out_of_range("Index out of bounds");
	}

	// return the degree value
	return this->degrees[node_id];
}




/**
 * \brief Getter for the weight of the edge between two given vertices.
 * 
 * \note A return value of zero means there is no edge between given vertices.
 * 
 * \param source ID of the beginning vertex of the edge.
 * \param destination ID of the end vertex of the edge.
 * \return Weight of the connection.
 */
const uint32_t Graph::List::get_edge(std::size_t source, std::size_t destination)
{
	uint32_t ret = 0;

	if (source >= this->list.size() || destination >= this->list.size())
	{
		throw std::out_of_range("Index out of bounds");
	}
	for (auto itr = this->list[source].begin(); itr != this->list[source].end(); itr++)
	{
		if (itr->ID == destination)
		{
			ret = itr->weight;
			break;
		}
	}

	return ret;
}




/**
 * \brief Getter for the name of the graph.
 * 
 * \return The name of the graph.
 */
const std::string Graph::List::get_name()
{
	return this->name;
}




/**
 * \brief Getter for the type of the graph.
 * 
 * \return The type of the graph as const.
 */
const Graph::Type Graph::List::get_type()
{
	return this->type;
}




/**
 * \brief Function saving current graph structure  into a .GRAPHML format file.
 * 
 * \param output_file_path Path to the output file.
 */
void Graph::List::save_graphml(std::string output_file_path)
{
	// opening output file
	std::ofstream file(output_file_path);

	// header
	file << "<?xml version=\"1.0\"";
	file << " encoding=\"UTF-8\"?>\n";

	// xml schema
	file << "<graphml xmlns=";
	file << "\"http://graphml.graphdrawing.org/xmlns\"\n";
	file << "	xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n";
	file << "	xsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns\n";
	file << "	http://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">\n";

	// weight data info
	file << "<key id=\"d0\" for=\"edge\"\n";
	file << "	attr.name=\"weight\" attr.type=\"integer\"/>\n";

	// graph type data
	file << " <graph id=";
	file << "\"" + this->name + "\"";
	file << " edgedefault=";
	switch (this->type)
	{
	case Type::directed:
		file << "\"directed\">\n";
		break;
	case Type::undirected:
		file << "\"undirected\">\n";
		break;
		// in case of undefined type, directed type is assumed
	case Type::undefined:
		file << "\"directed\">\n";
		break;
	}

	// vertices data
	for (std::size_t i = 0; i < this->list.size(); i++)
	{
		file << "	<node id=\"n" << i;
		file << "\"/>\n";
	}

	// edge data including weight
	for (std::size_t i = 0; i < this->list.size(); i++)
	{
		for (auto itr = this->list[i].begin(); itr != this->list[i].end(); itr++)
		{
			if (this->type == Type::undirected && itr->ID < i)
			{
				continue;
			}
			file << "	<edge source=\"n" << i;
			file << "\" target=\"n" << itr->ID;
			file << "\">\n";
			file << "		<data key=\"d0\">" + std::to_string(itr->weight) + "</data>\n";
			file << "	</edge>\n";
		}
	}

	// close the tags and the file
	file << "	</graph>\n";
	file << "</graphml>";
	file.close();

}