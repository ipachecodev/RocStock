#include "Inventory.h"

#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <cstddef>
#include <filesystem>

bool Inventory::addProduct(const Product& product)
{
    // A product code must identify only one product.
    if (findProduct(product.getCode()) != nullptr)
    {
        return false;
    }
    
    products.push_back(product);
    return true;
}

const Product* Inventory::findProduct(const std::string &code) const
{
    // Search the list for a matching code.
    for (const Product& product : products)
    {
        if (product.getCode() == code)
        {
            return &product;
        }
    }
    
    return nullptr;
}

bool Inventory::recordEntry(const std::string &code, int amount)
{
    // Use a non-const reference because the quantity will change.
    for (Product & product : products)
    {
        if (product.getCode() == code)
        {
            return product.addStock(amount);
        }
    }
    
    return false;
}

bool Inventory::recordExit(const std::string &code, int amount)
{
    // Product::removeStock prevents negative inventory.
    for (Product& product : products)
    {
        if (product.getCode() == code)
        {
            return product.removeStock(amount);
        }
    }
    
    return false;
}

const std::vector<Product>& Inventory::getProducts() const
{
    return products;
}

bool Inventory::saveToFile(const std::string &filePath) const
{
    const std::string temporaryPath = filePath + ".tmp";
    
    std::ofstream file(temporaryPath);
    
    if (!file)
    {
        return false;
    }
    
    file << "ROCSTOCK 1 " << products.size() << '\n';
    
    for (const Product& product : products)
    {
        file    << std::quoted(product.getCode()) << ' '
        << std::quoted(product.getName()) << ' '
        << std::quoted(product.getCategory()) << ' '
        << std::quoted(product.getUnit()) << ' '
        << product.getQuantity() << ' '
        << product.getMinimumStock() << ' '
        << product.getTargetStock() << '\n';
    }

    file.close();
    if (!file)
    {
        return false;
    }
    
    std::error_code error;
    std::filesystem::rename(temporaryPath, filePath, error);
    return !error;
}

bool Inventory::loadFromFile(const std::string &filePath)
{
    std::ifstream file(filePath);
    
    if (!file)
    {
        return false;
    }
    
    std::string identifier;
    int version;
    
    std::size_t expectedCount;
    
    if (!(file >> identifier >> version >> expectedCount) || identifier != "ROCSTOCK" || version != 1)
    {
        return false;
    }
    
    Inventory restored;
    std::string code, name, category, unit;
    int quantity, minimumStock, targetStock;
    
    for (std::size_t index = 0; index < expectedCount; ++index)
    {
        
        if (!(file >> std::quoted(code)))
        {
            return false;
        }
       
        if (!(file  >> std::quoted(name)
                    >> std::quoted(category)
                    >> std::quoted(unit)
                    >> quantity
                    >> minimumStock
                    >> targetStock))
        {
            return false;
        }
        
        try
        {
            Product product(code, name, category, unit, quantity, minimumStock, targetStock);
            
            if (!restored.addProduct(product))
            {
                return false;       // Duplicate code in the file.
            }
            
        }
        
        catch (const std::invalid_argument&)
        {
            return false;       // Invalid product data in the file.
        }
        
        
    }
    
    // Reject unexpected data after the declared number of products.
    file >> std::ws;
    if (!file.eof())
    {
        return false;
    }
    
    // Replace the current inventory only after every product was valid.
    products.swap(restored.products);
    return true;
}
