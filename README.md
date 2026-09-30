# RocStock

RocStock is a C++ console application for managing product inventory. It tracks products using unique codes, records stock changes, shows low-stock alerts, and creates a shopping list. The interface is available in English and Spanish.

## Features

- Add products with unique codes, categories, units, and stock limits.
- Edit a product's name, category, unit, and stock limits while keeping its unique code and quantity.
- Record stock entries and withdrawals.
- View low-stock and out-of-stock alerts.
- Generate a shopping list based on target stock levels.
- Switch the interface between English and Spanish.
- Save inventory automatically between sessions.

## Getting started

Requires macOS and Xcode with C++17 support.

1. Open `RocStock.xcodeproj` in Xcode.
2. Select the `RocStock` scheme and `My Mac`.
3. Press Run and choose English or Spanish in the console.

## Data storage

RocStock stores inventory locally at:

`~/Library/Application Support/RocStock/rocstock_data.txt`

The data file is created when inventory is saved. RocStock writes changes to a temporary file before replacing the saved inventory.
