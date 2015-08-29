// Copyright (C) 2011, 2012  Google Inc.
//
// This file is part of YouCompleteMe.
//
// YouCompleteMe is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// YouCompleteMe is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with YouCompleteMe.  If not, see <http://www.gnu.org/licenses/>.

#ifndef COMPLETIONDATA_H_2JCTF1NU
#define COMPLETIONDATA_H_2JCTF1NU

#include "standard.h"
#include <string>
#include <clang-c/Index.h>

namespace YouCompleteMe {

enum CompletionKind {
  STRUCT = 0,
  CLASS,
  ENUM,
  TYPE,
  MEMBER,
  FUNCTION,
  VARIABLE,
  MACRO,
  PARAMETER,
  NAMESPACE,
  UNKNOWN
};

// This class holds pieces of information about a single completion coming from
// clang. These pieces are shown in Vim's UI in different ways.
//
// Normally, the completion menu looks like this (without square brackets):
//
//   [main completion text]  [kind]  [extra menu info]
//   [main completion text]  [kind]  [extra menu info]
//   [main completion text]  [kind]  [extra menu info]
//    ... (etc.) ...
//
// The user can also enable a "preview" window that will show extra information
// about a completion at the top of the buffer.
struct CompletionData {
  CompletionData() {}
  CompletionData( const CXCompletionResult &completion_result );

  // What should actually be inserted into the buffer. For a function like
  // "int foo(int x)", this is just "foo". Same for a data member like "foo_":
  // we insert just "foo_".
  std::string TextToInsertInBuffer() {
    return call_string_;
  }

  // Currently, here we show the full function signature (without the return
  // type) if the current completion is a function or just the raw TypedText if
  // the completion is, say, a data member. So for a function like "int foo(int
  // x)", this would be "foo(int x)". For a data member like "count_", it would
  // be just "count_".
  std::string MainCompletionText() {
    return everything_except_return_type_;
  }

  // This is extra info shown in the pop-up completion menu, after the
  // completion text and the kind. Currently we put the return type of the
  // function here, if any.
  std::string ExtraMenuInfo() {
    return return_type_;
  }

  // This is used to show extra information in vim's preview window. This is the
  // window that vim usually shows at the top of the buffer. This should be used
  // for extra information about the completion.
  std::string DetailedInfoForPreviewWindow() {
    return detailed_info_;
  }

  std::string DocString() {
    return doc_string_;
  }

  std::string Brief() const {
    return brief_;
  }

  bool operator== ( const CompletionData &other ) const {
    return
      kind_ == other.kind_ &&
      everything_except_return_type_ == other.everything_except_return_type_ &&
      return_type_ == other.return_type_;
    // detailed_info_ doesn't matter
  }

  void AppendExtraSpaceIfNeeded() {
    call_string_.append( extra_space_ );
    everything_except_return_type_.append( extra_space_ );
  }

  static void EnableExtraSpace() {
    extra_space_ = " ";
  }

  static void DisableExtraSpace() {
    extra_space_ = "";
  }

  CompletionKind kind_;

  std::string everything_except_return_type_;

  std::string doc_string_;

  std::string call_string_;

  std::string key_string_;

  std::string brief_;

  std::string detailed_info_;

  std::string return_type_;

  static std::string extra_space_;

private:

  void ExtractDataFromChunk( CXCompletionString completion_string,
                             uint chunk_num,
                             bool &saw_left_paren,
                             bool &saw_function_params );
};

} // namespace YouCompleteMe


#endif /* end of include guard: COMPLETIONDATA_H_2JCTF1NU */
