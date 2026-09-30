#ifndef ROCKSTOCK_PRODUCT_H
#define ROCKSTOCK_PRODUCT_H

#include <string>

class Product
{
private:
    std::string code;       // Unique code used to find the product
    std::string name;       // Name shown in the inventory
    std::string category;   // Category chosen by the business
    std::string unit;       // Pieces, boxes, grams, milliliters, etc
    
    int quantity;           // Amount currently available
    int minimumStock;       // Amount at which a low-stock alert appears
    int targetStock;        // Amount the business wants to have after restocking
    
public:
    Product(const std::string& code,
            const std::string& name,
            const std::string& category,
            const std::string& unit,
            int quantity,
            int minimumStock,
            int targetStock);
    
    const std::string& getCode() const;
    const std::string& getName() const;
    const std::string& getCategory() const;
    const std::string& getUnit() const;
    int getQuantity() const;
    int getMinimumStock() const;
    int getTargetStock() const;
    
    bool updateDetails(const std::string& newName,
                       const std::string& newCategory,
                       const std::string& newUnit,
                       int newMinimumStock,
                       int newTargetStock);
    
    bool addStock(int amount);
    bool removeStock(int amount);
    bool isOutOfStock() const;
    bool isLowStock() const;
    int getAmountToOrder() const;
};

#endif /* Product_h */
