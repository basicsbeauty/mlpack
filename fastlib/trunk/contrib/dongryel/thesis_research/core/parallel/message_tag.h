/** @file message_tag.h
 *
 *  @author Dongryeol Lee (dongryel@cc.gatech.edu)
 */

#ifndef CORE_PARALLEL_MESSAGE_TAG_H
#define CORE_PARALLEL_MESSAGE_TAG_H

namespace core {
namespace parallel {

class MessageTag {
  public:
    enum { RECEIVE_SUBTABLE, REQUEST_LOAD_ESTIMATE, RECEIVE_LOAD_ESTIMATE, REQUEST_TASK_LIST, RECEIVE_TASK_LIST, MPI_PROCESS_DONE, MPI_PROCESS_DONE_ACK };
};
}
}

#endif
