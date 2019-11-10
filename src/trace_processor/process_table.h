/*
 * Copyright (C) 2018 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SRC_TRACE_PROCESSOR_PROCESS_TABLE_H_
#define SRC_TRACE_PROCESSOR_PROCESS_TABLE_H_

#include <limits>
#include <memory>

#include "src/trace_processor/sqlite/sqlite_table.h"
#include "src/trace_processor/trace_storage.h"

namespace perfetto {
namespace trace_processor {

// The implementation of the SQLite table containing each unique process with
// their details.
class ProcessTable : public SqliteTable {
 public:
  enum Column {
    kUpid = 0,
    kName = 1,
    kPid = 2,
    kStartTs = 3,
    kEndTs = 4,
    kParentUpid = 5
  };
  class Cursor : public SqliteTable::Cursor {
   public:
    Cursor(ProcessTable*);

    // Implementation of Table::Cursor.
    int Filter(const QueryConstraints&, sqlite3_value**) override;
    int Next() override;
    int Eof() override;
    int Column(sqlite3_context*, int N) override;

   private:
    const TraceStorage* const storage_;
    UniquePid min_ = 0;
    UniquePid max_ = 0;
    uint32_t index_ = 0;
    bool desc_ = false;
  };

  static void RegisterTable(sqlite3* db, const TraceStorage* storage);

  ProcessTable(sqlite3*, const TraceStorage*);

  // Table implementation.
  util::Status Init(int, const char* const*, SqliteTable::Schema*) override;
  std::unique_ptr<SqliteTable::Cursor> CreateCursor() override;
  int BestIndex(const QueryConstraints&, BestIndexInfo*) override;

 private:
  const TraceStorage* const storage_;
};
}  // namespace trace_processor
}  // namespace perfetto

#endif  // SRC_TRACE_PROCESSOR_PROCESS_TABLE_H_
