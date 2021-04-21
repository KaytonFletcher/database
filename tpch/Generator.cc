#include "Generator.h"

void TPCH::ParseConfigFile(std::unordered_map<std::string, Schema *> &schemas) {

  FILE *fp = fopen(settings.c_str(), "r");
  if (fp) {
    char *mem = (char *)malloc(80 * 3);
    char *temp_catalog_path = &mem[0];
    char *temp_dbfile_dir = &mem[80];
    char *temp_tpch_dir = &mem[160];
    char line[80];
    fgets(line, 80, fp);
    sscanf(line, "%s\n", temp_catalog_path);
    fgets(line, 80, fp);
    sscanf(line, "%s\n", temp_dbfile_dir);
    fgets(line, 80, fp);
    sscanf(line, "%s\n", temp_tpch_dir);
    fclose(fp);
    if (!(temp_catalog_path && temp_dbfile_dir && temp_tpch_dir)) {
      std::cerr << " Test settings file 'test.cat' not in correct format.\n";
      free(mem);
      exit(1);
    } else {
      std::string catalogPath = temp_catalog_path;
      std::string dbFilePath = temp_dbfile_dir;
      std::string tpchFilePath = temp_tpch_dir;

      std::cout
          << " \n** IMPORTANT: MAKE SURE THE INFORMATION BELOW IS CORRECT **\n";
      std::cout << " catalog location: \t" << catalogPath << std::endl;
      std::cout << " tpch files dir: \t" << tpchFilePath << std::endl;
      std::cout << " heap files dir: \t" << dbFilePath << std::endl;
      std::cout << " \n\n";

      schemas = {
          {"nation", new Schema(catalogPath.c_str(), nation.c_str())},
          {"region", new Schema(catalogPath.c_str(), region.c_str())},
          {"customer", new Schema(catalogPath.c_str(), customer.c_str())},
          {"part", new Schema(catalogPath.c_str(), part.c_str())},
          {"partsupp", new Schema(catalogPath.c_str(), partsupp.c_str())},
          {"supplier", new Schema(catalogPath.c_str(), supplier.c_str())},
          {"orders", new Schema(catalogPath.c_str(), orders.c_str())},
          {"lineitem", new Schema(catalogPath.c_str(), lineitem.c_str())}};

      free(mem);
    }
  } else {
    std::cerr << " Test settings files 'test.cat' missing \n";
    exit(1);
  }
}