/*
 * TODO: write CMAKE file
*/

#include <string>
#include <iostream>
#include <string_view>
#include <cstdint>

#include <lazycsv.hpp>
#include <avl_tree.hpp>

uint64_t atouint64(const std::string& str) { 
    uint64_t result = 0;

    int i=0;
    size_t length = str.length();
    for (char ch : str) {
        if (std::isdigit(ch)) {
            result += pow(10,(length - i - 1)) * (ch - 0x30);
            i++;
        }
    }
    return result;
}

int main(int argc, char **argv)
{
    tree_t *beer_database = NULL;
    
    lazycsv::parser parser{ "../brewery_data_complete_extended.csv" };
    int num_entries = 0;
    uint64_t tmp = 0;
    for (const auto row : parser)
    {
        /* LazyCSV row indices must be in ascending order */
        const auto [batch_id, beer_style, sku, location] = row.cells(0, 2, 3, 4); 

        /* Add new entry to dictionary */
        beer_t *beer = new beer_t();
        beer->batch_id = atouint64(std::string(batch_id.raw()));
        beer->sku = std::string(sku.raw());
        beer->beer_style = std::string(beer_style.raw());
        beer->location = std::string(location.raw());

        beer_database = avl_tree_insert(beer_database, (tree_entry_t)beer);

        num_entries++;
    }

#ifdef DEBUG
    avl_tree_print(beer_database,0,10);
#endif

    std::cout << "Read in the beer database " << num_entries << " entries" << std::endl;
    
    std::string user_input;
    while(true)
    {
        std::cout << "Enter a batch ID: ";
        std::getline(std::cin, user_input);

        beer_t *search_term = new beer_t();
        search_term->batch_id = atouint64(user_input);

        tree_entry_t dict_entry = avl_tree_lookup(beer_database, (tree_entry_t)search_term);
        if(dict_entry != NULL){
            beer_t *beer = (beer_t *)dict_entry;

            std::cout << "[" << beer->batch_id << "] : A " 
                    << beer->sku << " of " << beer->beer_style << " from "
                    << beer->location << std::endl << std::endl;
        }else{
            std::cout << "Not Found" << std::endl;
        }
    }
    return 0;
}