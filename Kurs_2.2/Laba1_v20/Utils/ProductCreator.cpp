#include "ProductCreator.h"

string CheckFields(const string fields_names[], const vector<string> &fields) {
    if (fields.size() >= sizeof(fields_names)) 
        return "";

    string error = "Not enough fields: ";
    
    for (int i = fields.size(); i < 5; i++)
        error += fields_names[i] + ", ";
    error.erase(error.length() - 2, 2);

    return error;
}

Product* ProductCreator::CreateProduct(const vector<string> &fields) {
    const int fields_count = 5;
    const string fields_names[] = {"Type", "Label", "Price", "Quantity", "Production date"};

    ProductData product_data;
    string error = "";

    // Missing fields
    error = CheckFields(fields_names, fields);
    if (!error.empty()) {PushError(error); return NULL;}

    // Label
    product_data.label = fields[1];

    // Price
    error = try_to_cast(fields_names[2], fields[2], product_data.price, std::stoi);
    if (!error.empty()) {PushError(error); return NULL;}
    
    // Quantity
    error = try_to_cast(fields_names[3], fields[3], product_data.quantity, std::stod);
    if (!error.empty()) {PushError(error); return NULL;}
    
    // Production Date
    error = parse_date(fields_names[4], fields[4], product_data.production_date);
    if (!error.empty()) {PushError(error); return NULL;}
    
    // Other fields
    vector<string> other_fields;
    other_fields.resize(fields.size() - fields_count);
    std::copy(fields.begin() + fields_count, fields.end(), other_fields.begin());

    // Creating product
    if (fields[0] == "beer")
        return CreateBeer(product_data, other_fields);
    if (fields[0] == "vodka")
        return CreateVodka(product_data, other_fields);
    if (fields[0] == "wine")
        return CreateWine(product_data, other_fields);

    PushError("Unknown product type '" + fields[0] + "'");
    return NULL;
}

Product* ProductCreator::CreateBeer(ProductData data, const vector<string> &fields) {
    const string fields_names[] = {"Expiration date", "Warehouse capacity"};
    const int fields_count = sizeof(fields_names);

    string error = "";

    // Missing fields
    error = CheckFields(fields_names, fields);
    if (!error.empty()) {PushError(error); return NULL;}

    // Expiration Date
    std::tm expiration_date;
    error = parse_date(fields_names[0], fields[0], expiration_date);
    if (!error.empty()) {PushError(error); return NULL;}

    // Warehouse capacity
    double warehouse_capacity;
    error = try_to_cast(fields_names[1], fields[1], warehouse_capacity, std::stod);
    if (!error.empty()) {PushError(error); return NULL;}

    return new Beer(data, expiration_date, warehouse_capacity);
}

Product* ProductCreator::CreateVodka(ProductData data, const vector<string> &fields) {
    const string fields_names[] = {"Warehouse capacity"};
    const int fields_count = sizeof(fields_names);
    
    string error = "";

    // Missing fields
    error = CheckFields(fields_names, fields);
    if (!error.empty()) {PushError(error); return NULL;}

    // Warehouse capacity
    double warehouse_capacity;
    error = try_to_cast(fields_names[0], fields[0], warehouse_capacity, std::stod);
    if (!error.empty()) {PushError(error); return NULL;}

    return new Vodka(data, warehouse_capacity);
}

Product* ProductCreator::CreateWine(ProductData data, const vector<string> &fields) {
    return new Wine(data);
}
