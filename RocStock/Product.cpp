#include "Product.h"

#include <limits>
#include <stdexcept>

Product::Product(const std::string& code,
                 const std::string& name,
                 const std::string& category,
                 const std::string& unit,
                 int quantity,
                 int minimumStock,
                 int targetStock)

    :   code(code),
        name(name),
        category(category),
        unit(unit),
        quantity(quantity),
        minimumStock(minimumStock),
        targetStock(targetStock)
{
    // Reject products without the information needed to identify them
    if (code.empty() || name.empty() || unit.empty())
    {
        throw std::invalid_argument("Code, name, and unit cannot be empty.");
    }
    
    // Stock amounts cannot be negative. The target must reach the minimum
    if (quantity < 0 || minimumStock < 0 || targetStock < minimumStock)
    {
        throw std::invalid_argument("Invalid stock amounts.");
    }
}

const std::string& Product::getCode() const
{
    return code;
}

const std::string& Product::getName() const
{
    return name;
}

const std::string& Product::getCategory() const
{
    return category;
}

const std::string& Product::getUnit() const
{
    return unit;
}

int Product::getQuantity() const
{
    return quantity;
}

int Product::getMinimumStock() const
{
    return minimumStock;
}

int Product::getTargetStock() const
{
    return targetStock;
}

bool Product::updateDetails(const std::string& newName,
                            const std::string& newCategory,
                            const std::string& newUnit,
                            int newMinimumStock,
                            int newTargetStock)

{
if (newName.empty() || newUnit.empty() || newMinimumStock < 0 || newTargetStock < newMinimumStock)
{
    return false;
}

    name = newName;
    category = newCategory;
    unit = newUnit;
    minimumStock = newMinimumStock;
    targetStock = newTargetStock;

    return true;
}

bool Product::addStock(int amount)
{
    // Reject zero, negative amounts, and numbers that would overflow int.
    if (amount <= 0 || amount > std::numeric_limits<int>::max() - quantity) {
        return false;
    }

    quantity += amount;
    return true;
}

bool Product::removeStock(int amount)
{
    // A sale or withdrawal cannot exceed the available amount.
    if (amount <= 0 || amount > quantity)
    {
        return false;
    }

    quantity -= amount;
    return true;
}

bool Product::isOutOfStock() const
{
    return quantity == 0;
}

bool Product::isLowStock() const
{
    // An empty product has its own "out of stock" status.
    return quantity > 0 && quantity <= minimumStock;
}

int Product::getAmountToOrder() const
{
    // Suggest only the amount needed to reach the target.
    return quantity < targetStock ? targetStock - quantity : 0;
}
