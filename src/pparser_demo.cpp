#include "pparser.hpp"
#include <iostream>
#include <string>
#include <vector>

// handler for the 'add' subcommand
int handle_add_command(const pparser::ParseResult &result) {
  std::cout << "executing command: " << result.command_path << std::endl;

  const std::vector<std::string> *files = result.get_pos_arg_list("files");
  const auto force =
      result.find_kw_arg_bool("force"); // optional '--force' flag

  if (files) {
    std::cout << "  files to add: ";
    for (size_t i = 0; i < files->size(); ++i) {
      std::cout << (*files)[i] << (i == files->size() - 1 ? "" : ", ");
    }
    std::cout << std::endl;
  }

  std::cout << "  force flag is " + std::string(force ? "set." : "not set.")
            << std::endl;

  return 0;
}

// handler for the 'commit' subcommand
int handle_commit_command(const pparser::ParseResult &result) {
  std::cout << "executing command: " << result.command_path << std::endl;

  const std::string *message =
      result.get_kw_arg_string("message"); // required '-m' option
  const bool *amend =
      result.get_kw_arg_bool("amend"); // optional '--amend' flag
  const bool *verbose =
      result.get_kw_arg_bool("verbose"); // optional '--verbose' flag
  if (!message) {
    std::cerr << "error: required '-m' option not provided." << std::endl;
    return 1;
  }

  std::cout << "  commit message: " << *message << std::endl;
  std::cout << "  amend flag is " + std::string(*amend ? "set." : "not set.")
            << std::endl;
  std::cout << "  verbose flag is " +
                   std::string(*verbose ? "set." : "not set.")
            << std::endl;

  return 0;
}

// --- main demo ---

int main(int argc, char *argv[]) {
  pparser::ArgumentParser parser("git_demo",
                                 "a simple git-like command line demo.");

  parser.get_root_command().add_keyword_arg("verbose", "-v", "--verbose",
                                        "enable verbose output",
                                        pparser::ArgType_Flag);

  std::shared_ptr<pparser::Command> add_cmd = std::make_shared<pparser::Command>(
      "add", "add file contents to the index");
  add_cmd
      ->add_positional_arg("files", "files to add", pparser::ArgType_Multiple,
                         true) // required, multiple values
      .add_keyword_arg("force", "-f", "--force", "allow adding ignored files",
                     pparser::ArgType_Flag, false)
      .set_handler(handle_add_command);

  std::shared_ptr<pparser::Command> commit_cmd =
      std::make_shared<pparser::Command>("commit",
                                         "record changes to the repository");
  commit_cmd
      ->add_keyword_arg("message", "-m", "--message", "commit message",
                      pparser::ArgType_Single, true) // required, single value
      .add_keyword_arg("amend", "-a", "--amend", "amend the previous commit",
                     pparser::ArgType_Flag, false)
      .add_keyword_arg("verbose", "-v", "--verbose", "enable verbose output",
                     pparser::ArgType_Flag, false)
      .set_handler(handle_commit_command);

  parser.get_root_command().add_command(add_cmd);
  parser.get_root_command().add_command(commit_cmd);

  pparser::ParseResult result;

  if (argc < 2) {
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "pparser demo (git-like cli)" << std::endl;
    std::cout << "enter commands, e.g.:" << std::endl;
    std::cout << "\tadd file1.txt -f" << std::endl;
    std::cout << "\tcommit -m \"initial\"" << std::endl;
    std::cout << "\thelp" << std::endl;
    std::cout << "\t--help" << std::endl;
    std::cout << "\tadd --help" << std::endl;
    std::cout << "type 'exit' or 'quit' to quit." << std::endl;
    std::cout << "----------------------------------------" << std::endl;

    std::string line;
    while (true) {
      std::cout << "> ";
      std::getline(std::cin, line);

      if (line.empty())
        continue;
      if (line == "exit" || line == "quit")
        break;

      std::cout << "--- [input]: " << line << " ---" << std::endl;
      std::cout << "--- [output] ---" << std::endl;
      result = parser.parse(line);
      std::cout << "----------------------------------------" << std::endl;
    }
  } else {
    std::cout << "--- [output] ---" << std::endl;
    result = parser.parse(argc, argv);
    std::cout << "----------------------------------------" << std::endl;
  }

  return (result.status == pparser::ParseResult::ParserStatus_ParseError)
             ? 1
             : 0;
}