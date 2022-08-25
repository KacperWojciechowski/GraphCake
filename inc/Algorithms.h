#pragma once

#include <cstdint>
#include <map>
#include <vector>

namespace Graph
{
	/**
	 * \brief Class for storing data from path-searching algorithms. 
	 * 
	 * \note The class does allow for storing several paths of the same length.
	 */
	class Roadmap
	{
	public:
		std::vector<std::vector<uint32_t>> prev_node; /**< Vector of possible previous vertices with the same distance */
		std::vector<uint32_t> distance; /**< The distance value from start vertex to any given vertex */
	
		void print();
	};

	/*
		Abstract algorithmic interface for the specific graph representations to implement
	*/

	/**
	 * \brief Virtual class providing the algorithmic interface.
	 * 
	 * The class provides common algorithmic interface for each of its inheriting
	 * classes. Its main purpose is to provide the Data::Benchmark class with an
	 * ability to use those functions without the knowledge of which graph representation
	 * was received as a parameter.
	 * 
	 * \attention Each of the coloring algorithms public function provide permutation
	 * functionality for randomized trials of efficiency. This functionality is triggered via a parameter
	 * to the chosen coloring function. As such, user might try to get an average result by running the function once,
	 * or try to get optimal results for the algorithm by running it multiple times with permutations, as does the
	 * Data::Benchmark class. For the latter purpose, the use of Data::Benchmark class is recommended, as it provides
	 * API to automate the process.
	 */
	class Algorithms
	{
	public:

		// algorithmic interface ------------------------------------------------------------------------------------------------

		/**
		 * Common definition of the function providing ordering and permutation sequence according to the greedy algorithm rules.
		 * 
		 * \param permutate Flag signifing whether the function is supposed to perform a random permutation of the ordering for the coloring.
		 *					False by default.
		 * \param log_stream Pointer to the output stream in which logs should be saved. If passed a nullptr, logs will not be produced.
		 *					 Nullptr by default.
		 * \return Number of colors used in the coloring process.
		 */
		virtual int32_t greedy_coloring(bool permutate = false, std::ostream* log_stream = nullptr) = 0;

		/**
		 * Common definition of the function providing ordering and permutation sequence according to the LF algorithm rules. 
		 * 
		 * \param permutate Flag signifing whether the function is supposed to perform a random permutation of the ordering for the coloring.
		 *					False by default.
		 * \param log_stream Pointer to the output stream in which logs should be saved. If passed a nullptr, logs will not be produced.
		 *					 Nullptr by default.
		 * \return Numer of colors used in the coloring process.
		 */
		virtual int32_t lf_coloring(bool permutate = false, std::ostream* log_stream = nullptr) = 0;

		/**
		 * Common definition of the function providing ordering and permutation sequence according to the SL algorithm rules.
		 * 
		 * \param permutate Flag signifing whether the function is supposed to perform a random permutation of the ordering for the coloring.
		 *					False by default.
		 * \param log_stream Pointer to the output stream in which logs should be saved. If passed a nullptr, logs will not be produced.
		 *					 Nullptr by default.
		 * \return Numer of colors used in the coloring process.
		 */
		virtual int32_t sl_coloring(bool permutate = false, std::ostream* log_stream = nullptr) = 0;

		/**
		 * Common definition of the function providing the Belman-Ford path-searching algorithm implementation.
		 * 
		 * \param start ID of the vertex (starting from 0) from which the search for paths to all other
		 * vertices should begin.
		 * \param permutate Flag signifing whether the function is supposed to perform a random permutation of the ordering for the coloring.
		 *					False by default.
		 * \return Data::Roadmap structure containing all necessary informations regarding the found paths.
		 */
		virtual Roadmap belman_ford_point_to_all(std::size_t start, std::ostream* log_stream = nullptr) = 0;

		/**
		 * Common definition of the function providing the Belman-Ford path-searching algorithm, with extraction of point-to-point path.
		 * 
		 * \param start ID of the vertex (starting from 0) from which to search for path to the destination vertex  
		 * \param destination ID of the destination vertex
		 * \param permutate Flag signifing whether the function is supposed to perform a random permutation of the ordering for the coloring.
		 *					False by default.
		 * \return 
		 */
		virtual std::vector<std::size_t> belman_ford_point_to_point(std::size_t start, std::size_t destination, std::ostream* log_stream = nullptr) = 0;

	protected:

		// coloring functions core ----------------------------------------------------------------------------------------------

		/**
		 * Common definition of the function providing the core coloring functionality.
		 * This functionality is designed for the algorithms that base on the greedy coloring sequence 
		 * (once provided the right order of vertices according to their own rules).
		 * 
		 * \param m Map containing the created ordering of vertices.
		 * \param permutate Flag signifing whether the function is supposed to perform a random permutation of the ordering for the coloring.
		 *					False by default.
		 * \return Number of colors used in the coloring process.
		 */
		virtual int32_t greedy_coloring_core(std::map<int, int>& m, std::ostream* log_stream = nullptr) = 0;
	};
}