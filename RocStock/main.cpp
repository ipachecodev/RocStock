#include "Inventory.h"
#include <sstream>
#include <limits>
#include <iostream>
#include <string>
#include <filesystem>
#include <cstdlib>

enum class Language
{
    English, Spanish
};

std::string translate(Language language, const std::string& english, const std::string& spanish)
{
    return language == Language::English ? english : spanish;
}

bool readNonNegativeInt(Language language,
                        const std::string& englishPrompt,
                        const std::string& spanishPrompt,
                        int& result)
{
    
    while (true)
    {
        std::cout << translate(language, englishPrompt, spanishPrompt);
        
        std::string line;
        if (!std::getline(std::cin, line))
        {
            return false;   // Input was closed.
        }
        
        std::istringstream input(line);
        long long number;
        char extra;
        
        // Accept one whole number, with no extra characters.
        if ((input >> number) &&
            !(input >> extra) &&
            number >= 0 &&
            number <= std::numeric_limits<int>::max())
        {
            result = static_cast<int>(number);
            return true;
        }
        
        std::cout << translate(language, "Enter a valid whole number (0 or greater).\n",
                               "Ingresa un número entero válido (0 o mayor).\n");
    }
}

void showMenu(Language language)
{
    std::cout << "\n===== ROCSTOCK =====\n";
    std::cout << translate(language, "1. Add product\n", "1. Agregar producto\n");
    std::cout << translate(language, "2. View inventory\n", "2. Consultar inventario\n");
    std::cout << translate(language, "3. Record stock in\n", "3. Registrar entrada\n");
    std::cout << translate(language, "4. Record stock out\n", "4. Registrar salida\n");
    std::cout << translate(language, "5. View alerts\n", "5. Ver alertas\n");
    std::cout << translate(language, "6. Shopping list\n", "6. Lista de compra\n");
    std::cout << translate(language, "7. Change language\n", "7. Cambiar idioma\n");
    std::cout << translate(language, "8. Edit product\n", "8. Editar producto\n");
    std::cout << translate(language, "0. Exit\n", "0. Salir\n");
    std::cout << translate(language, "Choose an option: ", "Selecciona una opcion: ");
}

void addProduct(Inventory& inventory, Language language)
{
    std::string code;
    std::string name;
    std::string category;
    std::string unit;
    
    std::cout << translate(language, "Product code: ", "Código del producto: ");
    if (!std::getline(std::cin, code)) return;
    
    // Stop inmediately if this code already belongs to a product.
    if (inventory.findProduct(code) != nullptr)
    {
        std::cout << translate(language, "A product with that code already exists.\n", "Ya existe un producto con ese código.\n"); return;
        
    }
    
    std::cout << translate(language, "Product name: ", "Nombre del producto: ");
    if (!std::getline(std::cin, name)) return;
    
    std::cout << translate(language, "Category (optional): ", "Categoría (opcional): ");
    if (!std::getline(std::cin, category)) return;
    
    std::cout << translate(language, "Unit (pieces, boxes, grams, etc,): ", "Unidad (piezas, cajas, gramos, etc.): ");
    if (!std::getline(std::cin, unit)) return;
    
    // The Product constructor requires these three fields.
    if (code.empty() || name.empty() || unit.empty())
    {
        std::cout << translate(language, "Code, name, and unit are requiered.\n", "El código nombre y unidad son obligatorios.\n");
        return;
    }
    
    int quantity;
    int minimumStock;
    int targetStock;
    
    if (!readNonNegativeInt(language, "Current quantity: ", "Cantidad actual: ", quantity))
        return;
    
    if(!readNonNegativeInt(language, "Minimum stock: ", "Inventario mínimo: ", minimumStock))
        return;
    
    // Keep asking until the target is at least the minimum.
    while (true)
    {
        if (!readNonNegativeInt(language, "Target stock: ", "Inventario objetivo: ", targetStock))
            return;
        
        if (targetStock >= minimumStock) break;
        
        std::cout << translate(language, "The target cannot be below the minimum.\n", "El objetivo no puede ser menor que el mínimo.\n");
    }
    
    Product product(code, name, category, unit, quantity, minimumStock, targetStock);
    
    // Inventory rejects another product with the same code.
    if (inventory.addProduct(product))
    {
        std::cout << translate(language, "Product added successfully.\n", "Producto agregado correctamente.\n");
    }
    
    else
    {
        std::cout << translate(language, "A product with that code already exists.\n", "Ya existe un producto con ese código.\n");
    }
    
}

void viewInventory(const Inventory& inventory, Language language)
{
    const auto& products = inventory.getProducts();
    
    if (products.empty())
    {
        std::cout << translate(language, "No products registered yet.\n", "Todavía no hay productos registrados.\n"); return;
    }
    
    std::cout << translate(language, "\n===== INVENTORY =====\n", "===== INVENTARIO =====\n");
    
    // Display each product without changing its stored information.
    for (const Product& product : products)
    {
        std::cout << "\n[" << product.getCode() << "] " << product.getName() << '\n';
        
        if (!product.getCategory().empty())
        {
            std::cout << translate(language, "Category: ", "Categoría: ") << product.getCategory() << '\n';
        }
        
        std::cout << translate(language, "Available: ", "Disponible: ") << product.getQuantity() << ' ' << product.getUnit() << '\n';

        std::cout << translate(language, "Minimum: ", "Mínimo: ") << product.getMinimumStock() << '\n';

        std::cout << translate(language, "Target: ", "Objetivo: ") << product.getTargetStock() << '\n';

        if (product.isOutOfStock())
        {
        std::cout << translate(language, "Status: OUT OF STOCK\n", "Estado: AGOTADO\n");
        }
                
        else if (product.isLowStock())
        {
        std::cout << translate(language, "Status: LOW STOCK\n", "Estado: INVENTARIO BAJO\n");
        }
                
        else
        {
            std::cout << translate(language, "Status: AVAILABLE\n", "Estado: DISPONIBLE\n");
        }
                    
    }
}

void recordStockIn(Inventory& inventory, Language language)
{
    std::string code;
    
    std::cout << translate(language, "Product code: ", "Código del producto: ");
    if(!std::getline(std::cin, code)) return;
    
    // Check that the product exists before asking for an amount.
    if (inventory.findProduct(code) == nullptr)
    {
        std::cout << translate(language, "Product not found.\n", "Producto no encontrado.\n"); return;
    }
    
    int amount;
    if (!readNonNegativeInt(language, "Amount received: ", "Cantidad recibida: ", amount)) return;
    
    if (amount == 0)
    {
        std::cout << translate(language, "The amount must be greater than zero.\n", "La cantidad debe ser mayor que cero.\n"); return;
    }
    
    // recordEntry also protects against a number too large for int.
    if (!inventory.recordEntry(code, amount))
    {
        std::cout << translate(language, "Could not record that amount.\n", "No se pudo registrar esa cantidad.\n"); return;
    }
    
    const Product* product = inventory.findProduct(code);
    
    std::cout << translate(language, "Entry recorded, Now available: ", "Entrada registrada. Ahora hay: ") << product->getQuantity() << ' ' << product->getUnit() << '\n';
}

void recordStockOut(Inventory& inventory, Language language)
{
    std::string code;

    std::cout << translate(language, "Product code: ", "Código del producto: ");
    if (!std::getline(std::cin, code)) return;

    const Product* product = inventory.findProduct(code);
    if (product == nullptr)
    {
        std::cout << translate(language, "Product not found.\n", "Producto no encontrado.\n"); return;
    }

    int amount;
    if (!readNonNegativeInt(language, "Amount withdrawn: ", "Cantidad retirada: ", amount)) return;

    if (amount == 0)
    {
        std::cout << translate(language, "The amount must be greater than zero.\n", "La cantidad debe ser mayor que cero.\n"); return;
    }

    // Never allow a withdrawal greater than the available quantity.
    if (amount > product->getQuantity())
    {
        std::cout << translate(language, "Insufficient stock. Available: ", "Inventario insuficiente. Disponible: ") << product->getQuantity() << ' ' << product->getUnit() << '\n'; return;
    }

    if (!inventory.recordExit(code, amount))
    {
        std::cout << translate(language, "Could not record the withdrawal.\n", "No se pudo registrar la salida.\n"); return;
    }

    std::cout << translate(language, "Withdrawal recorded. Now available: ", "Salida registrada. Ahora hay: ") << product->getQuantity() << ' ' << product->getUnit() << '\n';

    // Show an alert as soon as the new quantity reaches a critical level.
    if (product->isOutOfStock())
    {
        std::cout << translate(language, "ALERT: Product is out of stock.\n", "ALERTA: Producto agotado.\n");
    }
    
    else if (product->isLowStock())
    {
        std::cout << translate(language, "ALERT: Stock is low.\n", "ALERTA: Inventario bajo.\n");
    }
}

void viewAlerts(const Inventory& inventory, Language language)
{
    const auto& products = inventory.getProducts();
    bool foundAlert = false;
    
    std::cout << translate(language, "\n===== STOCK ALERTS =====\n", "===== ALERTAS DE INVENTARIO =====\n");
    
    // Only show products that need attention.
    for (const Product& product : products)
    {
        if (product.isOutOfStock())
        {
            foundAlert = true;
            std::cout   << "[" << product.getCode() << "] "
                        << product.getName()
                        << translate(language, " - OUT OF STOCK\n", " - AGOTADO\n");
        }
        
        else if (product.isLowStock())
        {
            foundAlert = true;
            std::cout   << "[" << product.getCode() << "] "
                        << product.getName()
                        << translate(language, " - LOW: ", " - BAJO: ")
                        << product.getQuantity() << ' '
                        << product.getUnit() << '\n';
        }
    }
    
    if (!foundAlert)
    {
        std::cout << translate(language, "No stock alerts right now.\n", "No hay alertas de inventario ahora mismo.\n");
    }
}

void viewShoppingList(const Inventory& inventory, Language language)
{
    if (inventory.getProducts().empty())
    {
        std::cout << translate(language, "No products registered yet.\n", "Todavía no hay productos registrados.\n"); return;
    }
    
    std::cout << translate(language, "\n===== SHOPPING LIST =====\n", "\n===== LISTA DE COMPRAS =====\n");
    
    bool hasItems = false;
    
    for (const Product& product : inventory.getProducts())
    {
        const int missing = product.getAmountToOrder();
        
        if (missing > 0)
        {
            hasItems = true;
            
            std::cout << "[" << product.getCode() << "] " << product.getName() << translate(language, " - buy ", " - comprar ") << missing << ' ' << product.getUnit() << '\n';
        }
    }
    
    if (!hasItems)
    {
        std::cout << translate(language, "Nothing to buy right now.\n", "No hace falta comprar nada ahora mismo.\n");
    }
}

bool editProduct(Inventory& inventory, Language language)
{
    std::string code;
    std::cout << translate(language, "Product code to edit: ", "Código del producto a editar: ");
    
    if (!std::getline(std::cin, code)) return false;
    
    const Product* product = inventory.findProduct(code);
    
    if (product == nullptr)
    {
        std::cout << translate(language, "Product not found.\n", "Producto no encontrado.\n");
        
        return false;
    }
    
    std::string newName;
    std::cout << translate(language, "New product name: ", "Nuevo nombre del producto: ");
    
    if (!std::getline(std::cin, newName)) return false;
    
    std::string newCategory;
    std::cout << translate(language, "New category (optional): ", "Nueva categoría (opcional): ");
    
    if (!std::getline(std::cin, newCategory)) return false;
    
    std::string newUnit;
    std::cout << translate(language, "New unit (pieces, boxes, etc.): ", "Nueva unidad (piezas, cajas, etc.): ");
    
    if (!std::getline(std::cin, newUnit)) return false;
    
    int newMinimumStock;
    
    if(!readNonNegativeInt(language, "New minimum stock: ", "Nuevo inventario mínimo: ", newMinimumStock)) return false;
    
    int newTargetStock;
    
    if(!readNonNegativeInt(language, "New target stock: ", "Nuevo inventario objetivo: ", newTargetStock)) return false;
    
    if (newName.empty() || newUnit.empty() ||  newTargetStock < newMinimumStock)
    {
        std::cout << translate(language, "Invalid product details.\n", "Datos del producto no válidos.\n"); return false;
    }
    
    return inventory.updateProduct(code, newName, newCategory, newUnit, newMinimumStock, newTargetStock);
}

int main ()
{
    Language language;
    Inventory inventory;
    std::string input;
    
    while (true)
    {
        std::cout << "Choose a language / Selecciona un idioma:\n";
        std::cout << "1. English\n2. Español\n";
        std::cout << "> ";
        
        if (!std::getline(std::cin, input))
        {
            return 0;
        }
        
        if (input == "1")
        {
            language = Language::English;
            break;
        }
        
        if (input == "2")
        {
            language = Language::Spanish;
            break;
        }
        
        std::cout << "Invalid option / Opción no válida.\n\n";
    }
    
    const std::filesystem::path dataDirectory = std::filesystem::path(std::getenv("HOME")) / "Library" / "Application Support" / "RocStock";
    
    std::error_code directoryError;
    std::filesystem::create_directories(dataDirectory, directoryError);
    if (directoryError)
    {
        std::cerr << translate(language, "Could not create the data folder.\n", "No se pudo crear la carpeta de datos.\n"); return 1;
    }
    
    const std::string dataFile = (dataDirectory / "rocstock_data.txt").string();
    
    if (std::filesystem::exists(dataFile) && !inventory.loadFromFile(dataFile))
    {
        std::cerr << translate(language, "Could not read the inventory file.\n", "No se pudo leer el archivo de inventario.\n");
        
        return 1;
    }
    
    while (true)
    {
        showMenu(language);
        
        if (!std::getline(std::cin, input))
        {
            break;
        }
        
        if (input == "0")
        {
            std::cout << translate(language, "Goodbye!\n", "¡Hasta Luego!\n");
            break;
        }
        
        else if (input == "7")
        {
            language = language == Language::English
                    ? Language::Spanish
                    : Language::English;
        }
        
        else if (input == "1")
        {
            addProduct(inventory, language);
            
            if (!inventory.saveToFile(dataFile))
            {
                std::cerr <<translate(language, "Could not save the inventory.\n", "No se pudo guardar el inventario.\n");
            }
        }
        
        else if (input == "2")
        {
            viewInventory(inventory, language);
        }
        
        else if (input == "3")
        {
            recordStockIn(inventory, language);
            
            if (!inventory.saveToFile(dataFile))
            {
                std::cerr << translate(language, "Could not save the inventory.\n", "No se pudo guardar el inventario.\n");
            }
            
        }
        
        else if (input == "4")
        {
            recordStockOut(inventory, language);
            
            if (!inventory.saveToFile(dataFile))
            {
                std::cerr << translate(language, "Could not save the inventory.\n", "No se pudo guardar el inventario.\n");
            }
        }
        
        else if (input == "5")
        {
            viewAlerts(inventory, language);
        }
        
        else if (input == "6")
        {
            viewShoppingList(inventory, language);
        }
        
        else if (input == "8")
        {
            if (editProduct(inventory, language))
            {
                std::cout << translate(language, "Product updated.\n", "Producto actualizado.\n");
                
                if (!inventory.saveToFile(dataFile))
                {
                    std::cerr << translate(language, "Could not save the inventory.\n", "No se pudo guardar el inventario.\n");
                }
            }
        }
        
        else
        {
            std::cout << translate(language, "Invalid option. Try again.\n", "Opción no válida. Intenta de nuevo.\n");
        }
    }
    
    return 0;
}
