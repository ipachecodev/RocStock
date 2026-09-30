#ifndef ROCSTOCK_INVENTORY_H
#define ROCSTOCK_INVENTORY_H

#include "Product.h"

#include <string>
#include <vector>

class Inventory
{
private:
    // Stores all products registered by the business.
    std::vector<Product> products;
    
public:
    // Adds a product only if its code is not already in use.
    bool addProduct(const Product& product);
    
    // Returns the matching product, or nullptr if it does not exist.
    const Product* findProduct(const std::string& code) const;
    
    bool updateProduct(const std::string& code,
                       const std::string& newName,
                       const std::string& newCategory,
                       const std::string& newUnit,
                       int newMinimumStock,
                       int newTargetStock);
    
    // Changes the quantity of an existing product.
    // Returns false if the code or amount is invalid
    bool recordEntry(const std::string& code, int amount);
    bool recordExit(const std::string& code, int amount);
    
    // Provides read-only access for inventory reports and alerts.
    const std::vector<Product>& getProducts() const;
    
    // Saves all products and their current quantities to a file.
    bool saveToFile(const std::string& filePath) const;
    
    // Restores products from a previously saved file.
    bool loadFromFile(const std::string& filePath);
    
};


#endif /* Inventory_h */
