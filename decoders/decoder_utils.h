#ifndef DECODER_UTILS_H_
#define DECODER_UTILS_H_

#include <utility>
#include "fst/log.h"
#include "path_trie.h"

const float NUM_FLT_INF = std::numeric_limits<float>::max();
const float NUM_FLT_MIN = std::numeric_limits<float>::min();

// inline function for validation check
inline void val_check(
    bool x, const char *expr, const char *file, int line, const char *err) {
  if (!x) {
    std::cout << "[" << file << ":" << line << "] ";
    LOG(FATAL) << "\"" << expr << "\" check failed. " << err;
  }
}

#define VALID_CHECK(x, info) \
  val_check(static_cast<bool>(x), #x, __FILE__, __LINE__, info)
#define VALID_CHECK_EQ(x, y, info) VALID_CHECK((x) == (y), info)
#define VALID_CHECK_GT(x, y, info) VALID_CHECK((x) > (y), info)
#define VALID_CHECK_LT(x, y, info) VALID_CHECK((x) < (y), info)


// Function template for comparing two pairs
template <typename T1, typename T2>
bool pair_comp_first_rev(const std::pair<T1, T2> &a,
                         const std::pair<T1, T2> &b) {
  return a.first > b.first;
}

// Function template for comparing two pairs
template <typename T1, typename T2>
bool pair_comp_second_rev(const std::pair<T1, T2> &a,
                          const std::pair<T1, T2> &b) {
  return a.second > b.second;
}

// Return the sum of two probabilities in log scale
template <typename T>
T log_sum_exp(const T &x, const T &y) {
  static T num_min = -std::numeric_limits<T>::max();
  if (x <= num_min) return y;
  if (y <= num_min) return x;
  T xmax = std::max(x, y);
  return std::log(std::exp(x - xmax) + std::exp(y - xmax)) + xmax;
}
// Return the difference of two probabilities in log scale
template <typename T>
T log_sub_exp(const T& x, const T& y) {
    if (x <= y)
        // error!! computing the log of a negative number
        if (y == -std::numeric_limits<T>::max())
            return x;
    return x + std::log1p(-std::exp(y - x));
}
// Return the product of two probabilities in log scale
template <typename T>
T log_prod_exp(const T& x, const int& y) {
    if (y == 0)
        return 0;

    T ans = x;
    for (int i = 0; i < y - 1; ++i)
        ans = log_sum_exp(ans, x);

    //TODO this can be implemented through powers of 2
    return ans;
}

// Get pruned probability vector for each time step's beam search
std::vector<std::pair<size_t, float>> get_pruned_log_probs(
    const std::vector<double> &prob_step,
    double cutoff_prob,
    size_t cutoff_top_n);

// Get beam search result from prefixes in trie tree
std::vector<std::pair<double, std::string>> get_beam_search_result(
    const std::vector<PathTrie *> &prefixes,
    const std::vector<std::string> &vocabulary,
    size_t beam_size,
    std::vector<std::tuple<std::string, uint32_t, uint32_t>>& wordlist);

std::vector<std::pair<double, std::string>> get_beam_search_result_kw(
    const std::vector<PathTrie*>& prefixes,
    const std::vector<std::string>& vocabulary,
    size_t beam_size,
    std::vector<std::tuple<std::string, uint32_t, uint32_t>>& wordlist);

// Functor for prefix comparsion
bool prefix_compare(const PathTrie *x, const PathTrie *y);


/* Get length of utf8 encoding string
 * See: http://stackoverflow.com/a/4063229
 */
size_t get_utf8_str_len(const std::string &str);

/* Split a string into a list of strings on a given string
 * delimiter. NB: delimiters on beginning / end of string are
 * trimmed. Eg, "FooBarFoo" split on "Foo" returns ["Bar"].
 */
std::vector<std::string> split_str(const std::string &s,
                                   const std::string &delim);

/* Splits string into vector of strings representing
 * UTF-8 characters (not same as chars)
 */
std::vector<std::string> split_utf8_str(const std::string &str);

// Add a word in index to the dicionary of fst
void add_word_to_fst(const std::vector<int> &word,
                     fst::StdVectorFst *dictionary);

// Add a word in string to dictionary
bool add_word_to_dictionary(
    const std::string &word,
    const std::unordered_map<std::string, int> &char_map,
    bool add_space,
    int SPACE_ID,
    fst::StdVectorFst *dictionary);
#endif  // DECODER_UTILS_H
