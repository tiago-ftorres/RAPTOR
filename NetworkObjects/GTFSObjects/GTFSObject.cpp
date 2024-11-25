//
// Created by maria on 11/20/2024.
//

#include "GTFSObject.h"

void GTFSObject::setField(const std::string &field, const std::string &value) {
  fields[field] = value;
}

// Get a field value
const std::string &GTFSObject::getField(const std::string &field) const {
  auto it = fields.find(field);
  if (it == fields.end()) {
    throw std::runtime_error("Field '" + field + "' not found.");
  }
  return it->second;
}
