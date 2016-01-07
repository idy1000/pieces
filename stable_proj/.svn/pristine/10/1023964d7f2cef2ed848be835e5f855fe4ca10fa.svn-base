//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2012. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_WINDOWS_MESSAGE_QUEUE_HPP
#define BOOST_INTERPROCESS_WINDOWS_MESSAGE_QUEUE_HPP

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

#include <boost/interprocess/windows_shared_memory.hpp> 
#include <boost/interprocess/ipc/message_queue.hpp>
#include <boost/interprocess/detail/managed_open_or_create_impl.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/detail/utilities.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/interprocess/permissions.hpp>
#include <boost/detail/no_exceptions_support.hpp>
#include <boost/interprocess/detail/type_traits.hpp>
#include <boost/intrusive/pointer_traits.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <boost/intrusive/pointer_traits.hpp>
#include <algorithm> //std::lower_bound
#include <cstddef>   //std::size_t
#include <cstring>   //memcpy


//!\file
//!Describes an inter-process message queue. This class allows sending
//!messages between processes and allows blocking, non-blocking and timed
//!sending and receiving.

namespace boost{  namespace interprocess{

//!A class that allows sending messages
//!between processes.
template<class VoidPointer>
class windows_message_queue_t
{
   /// @cond
   //Blocking modes
   enum block_t   {  blocking,   timed,   non_blocking   };

   windows_message_queue_t();
   /// @endcond

   public:
   typedef VoidPointer                                                 void_pointer;
   typedef typename boost::intrusive::
      pointer_traits<void_pointer>::template
         rebind_pointer<char>::type                                    char_ptr;
   typedef typename boost::intrusive::pointer_traits<char_ptr>::difference_type difference_type;
   typedef typename boost::make_unsigned<difference_type>::type        size_type;

   //!Creates a process shared message queue with name "name". For this message queue,
   //!the maximum number of messages will be "max_num_msg" and the maximum message size
   //!will be "max_msg_size". Throws on error and if the queue was previously created.
   windows_message_queue_t(create_only_t create_only,
                 const char *name,
                 size_type max_num_msg,
                 size_type max_msg_size,
                 const permissions &perm = permissions());

   //!Opens or creates a process shared message queue with name "name".
   //!If the queue is created, the maximum number of messages will be "max_num_msg"
   //!and the maximum message size will be "max_msg_size". If queue was previously
   //!created the queue will be opened and "max_num_msg" and "max_msg_size" parameters
   //!are ignored. Throws on error.
   windows_message_queue_t(open_or_create_t open_or_create,
                 const char *name,
                 size_type max_num_msg,
                 size_type max_msg_size,
                 const permissions &perm = permissions());

   //!Opens a previously created process shared message queue with name "name".
   //!If the queue was not previously created or there are no free resources,
   //!throws an error.
   windows_message_queue_t(open_only_t open_only,
                 const char *name);

   //!Destroys *this and indicates that the calling process is finished using
   //!the resource. All opened message queues are still
   //!valid after destruction. The destructor function will deallocate
   //!any system resources allocated by the system for use by this process for
   //!this resource. The resource can still be opened again calling
   //!the open constructor overload. To erase the message queue from the system
   //!use remove().
   ~windows_message_queue_t();

   //!Sends a message stored in buffer "buffer" with size "buffer_size" in the
   //!message queue with priority "priority". If the message queue is full
   //!the sender is blocked. Throws interprocess_error on error.
   void send (const void *buffer,     size_type buffer_size,
              unsigned int priority);

   //!Sends a message stored in buffer "buffer" with size "buffer_size" through the
   //!message queue with priority "priority". If the message queue is full
   //!the sender is not blocked and returns false, otherwise returns true.
   //!Throws interprocess_error on error.
   bool try_send    (const void *buffer,     size_type buffer_size,
                         unsigned int priority);

   //!Sends a message stored in buffer "buffer" with size "buffer_size" in the
   //!message queue with priority "priority". If the message queue is full
   //!the sender retries until time "abs_time" is reached. Returns true if
   //!the message has been successfully sent. Returns false if timeout is reached.
   //!Throws interprocess_error on error.
   bool timed_send    (const void *buffer,     size_type buffer_size,
                           unsigned int priority,  const boost::posix_time::ptime& abs_time);

   //!Receives a message from the message queue. The message is stored in buffer
   //!"buffer", which has size "buffer_size". The received message has size
   //!"recvd_size" and priority "priority". If the message queue is empty
   //!the receiver is blocked. Throws interprocess_error on error.
   void receive (void *buffer,           size_type buffer_size,
                 size_type &recvd_size,unsigned int &priority);

   //!Receives a message from the message queue. The message is stored in buffer
   //!"buffer", which has size "buffer_size". The received message has size
   //!"recvd_size" and priority "priority". If the message queue is empty
   //!the receiver is not blocked and returns false, otherwise returns true.
   //!Throws interprocess_error on error.
   bool try_receive (void *buffer,           size_type buffer_size,
                     size_type &recvd_size,unsigned int &priority);

   //!Receives a message from the message queue. The message is stored in buffer
   //!"buffer", which has size "buffer_size". The received message has size
   //!"recvd_size" and priority "priority". If the message queue is empty
   //!the receiver retries until time "abs_time" is reached. Returns true if
   //!the message has been successfully sent. Returns false if timeout is reached.
   //!Throws interprocess_error on error.
   bool timed_receive (void *buffer,           size_type buffer_size,
                       size_type &recvd_size,unsigned int &priority,
                       const boost::posix_time::ptime &abs_time);

   //!Returns the maximum number of messages allowed by the queue. The message
   //!queue must be opened or created previously. Otherwise, returns 0.
   //!Never throws
   size_type get_max_msg() const;

   //!Returns the maximum size of message allowed by the queue. The message
   //!queue must be opened or created previously. Otherwise, returns 0.
   //!Never throws
   size_type get_max_msg_size() const;

   //!Returns the number of messages currently stored.
   //!Never throws
   size_type get_num_msg();

   //!Removes the message queue from the system.
   //!Returns false on error. Never throws
   static bool remove(const char *name);

   /// @cond
   private:
   typedef boost::posix_time::ptime ptime;

   friend class ipcdetail::msg_queue_initialization_func_t<VoidPointer>;

   bool do_receive(block_t block,
                   void *buffer,         size_type buffer_size,
                   size_type &recvd_size, unsigned int &priority,
                   const ptime &abs_time);

   bool do_send(block_t block,
                const void *buffer,      size_type buffer_size,
                unsigned int priority,   const ptime &abs_time);

   //!Returns the needed memory size for the shared message queue.
   //!Never throws
   static size_type get_mem_size(size_type max_msg_size, size_type max_num_msg);
   ipcdetail::managed_open_or_create_impl<windows_shared_memory, 0, false, false> m_shmem;
   /// @endcond
};

/// @cond

template<class VoidPointer>
inline windows_message_queue_t<VoidPointer>::~windows_message_queue_t()
{}

template<class VoidPointer>
inline typename windows_message_queue_t<VoidPointer>::size_type windows_message_queue_t<VoidPointer>::get_mem_size
   (size_type max_msg_size, size_type max_num_msg)
{  return ipcdetail::mq_hdr_t<VoidPointer>::get_mem_size(max_msg_size, max_num_msg);   }

template<class VoidPointer>
inline windows_message_queue_t<VoidPointer>::windows_message_queue_t(create_only_t,
                                    const char *name,
                                    size_type max_num_msg,
                                    size_type max_msg_size,
                                    const permissions &perm)
      //Create shared memory and execute functor atomically
   :  m_shmem(create_only,
              name,
              get_mem_size(max_msg_size, max_num_msg),
              read_write,
              static_cast<void*>(0),
              //Prepare initialization functor
              ipcdetail::msg_queue_initialization_func_t<VoidPointer> (max_num_msg, max_msg_size),
              perm)
{}

template<class VoidPointer>
inline windows_message_queue_t<VoidPointer>::windows_message_queue_t(open_or_create_t,
                                    const char *name,
                                    size_type max_num_msg,
                                    size_type max_msg_size,
                                    const permissions &perm)
      //Create shared memory and execute functor atomically
   :  m_shmem(open_or_create,
              name,
              get_mem_size(max_msg_size, max_num_msg),
              read_write,
              static_cast<void*>(0),
              //Prepare initialization functor
              ipcdetail::msg_queue_initialization_func_t<VoidPointer> (max_num_msg, max_msg_size),
              perm)
{}

template<class VoidPointer>
inline windows_message_queue_t<VoidPointer>::windows_message_queue_t(open_only_t, const char *name)
   //Create shared memory and execute functor atomically
   :  m_shmem(open_only,
              name,
              read_write,
              static_cast<void*>(0),
              //Prepare initialization functor
              ipcdetail::msg_queue_initialization_func_t<VoidPointer> ())
{}

template<class VoidPointer>
inline void windows_message_queue_t<VoidPointer>::send
   (const void *buffer, size_type buffer_size, unsigned int priority)
{  this->do_send(blocking, buffer, buffer_size, priority, ptime()); }

template<class VoidPointer>
inline bool windows_message_queue_t<VoidPointer>::try_send
   (const void *buffer, size_type buffer_size, unsigned int priority)
{  return this->do_send(non_blocking, buffer, buffer_size, priority, ptime()); }

template<class VoidPointer>
inline bool windows_message_queue_t<VoidPointer>::timed_send
   (const void *buffer, size_type buffer_size
   ,unsigned int priority, const boost::posix_time::ptime &abs_time)
{
   if(abs_time == boost::posix_time::pos_infin){
      this->send(buffer, buffer_size, priority);
      return true;
   }
   return this->do_send(timed, buffer, buffer_size, priority, abs_time);
}

template<class VoidPointer>
inline bool windows_message_queue_t<VoidPointer>::do_send(block_t block,
                                const void *buffer,      size_type buffer_size,
                                unsigned int priority,   const boost::posix_time::ptime &abs_time)
{
   ipcdetail::mq_hdr_t<VoidPointer> *p_hdr = static_cast<ipcdetail::mq_hdr_t<VoidPointer>*>(m_shmem.get_user_address());
   //Check if buffer is smaller than maximum allowed
   if (buffer_size > p_hdr->m_max_msg_size) {
      throw interprocess_exception(size_error);
   }

   bool was_empty = false;
   //---------------------------------------------
   scoped_lock<interprocess_mutex> lock(p_hdr->m_mutex);
   //---------------------------------------------
   {
      //If the queue is full execute blocking logic
      if (p_hdr->is_full()) {
         switch(block){
            case non_blocking :
               return false;
            break;

            case blocking :
               do{
                  p_hdr->m_cond_send.wait(lock);
               }
               while (p_hdr->is_full());
            break;

            case timed :
               do{
                  if(!p_hdr->m_cond_send.timed_wait(lock, abs_time)){
                     if(p_hdr->is_full())
                        return false;
                     break;
                  }
               }
               while (p_hdr->is_full());
            break;
            default:
            break;
         }
      }

      was_empty = p_hdr->is_empty();
      //Insert the first free message in the priority queue
      ipcdetail::msg_hdr_t<VoidPointer> &free_msg_hdr = p_hdr->queue_free_msg(priority);

      //Sanity check, free msgs are always cleaned when received
      assert(free_msg_hdr.priority == 0);
      assert(free_msg_hdr.len == 0);

      //Copy control data to the free message
      free_msg_hdr.priority = priority;
      free_msg_hdr.len      = buffer_size;

      //Copy user buffer to the message
      std::memcpy(free_msg_hdr.data(), buffer, buffer_size);
   }  // Lock end

   //Notify outside lock to avoid contention. This might produce some
   //spurious wakeups, but it's usually far better than notifying inside.
   //If this message changes the queue empty state, notify it to receivers
   if (was_empty){
      p_hdr->m_cond_recv.notify_one();
   }

   return true;
}

template<class VoidPointer>
inline void windows_message_queue_t<VoidPointer>::receive(void *buffer,        size_type buffer_size,
                        size_type &recvd_size,   unsigned int &priority)
{  this->do_receive(blocking, buffer, buffer_size, recvd_size, priority, ptime()); }

template<class VoidPointer>
inline bool
   windows_message_queue_t<VoidPointer>::try_receive(void *buffer,              size_type buffer_size,
                              size_type &recvd_size,   unsigned int &priority)
{  return this->do_receive(non_blocking, buffer, buffer_size, recvd_size, priority, ptime()); }

template<class VoidPointer>
inline bool
   windows_message_queue_t<VoidPointer>::timed_receive(void *buffer,            size_type buffer_size,
                                size_type &recvd_size,   unsigned int &priority,
                                const boost::posix_time::ptime &abs_time)
{
   if(abs_time == boost::posix_time::pos_infin){
      this->receive(buffer, buffer_size, recvd_size, priority);
      return true;
   }
   return this->do_receive(timed, buffer, buffer_size, recvd_size, priority, abs_time);
}

template<class VoidPointer>
inline bool
   windows_message_queue_t<VoidPointer>::do_receive(block_t block,
                          void *buffer,            size_type buffer_size,
                          size_type &recvd_size,   unsigned int &priority,
                          const boost::posix_time::ptime &abs_time)
{
   ipcdetail::mq_hdr_t<VoidPointer> *p_hdr = static_cast<ipcdetail::mq_hdr_t<VoidPointer>*>(m_shmem.get_user_address());
   //Check if buffer is big enough for any message
   if (buffer_size < p_hdr->m_max_msg_size) {
      throw interprocess_exception(size_error);
   }

   bool was_full = false;
   //---------------------------------------------
   scoped_lock<interprocess_mutex> lock(p_hdr->m_mutex);
   //---------------------------------------------
   {
      //If there are no messages execute blocking logic
      if (p_hdr->is_empty()) {
         switch(block){
            case non_blocking :
               return false;
            break;

            case blocking :
               do{
                  p_hdr->m_cond_recv.wait(lock);
               }
               while (p_hdr->is_empty());
            break;

            case timed :
               do{
                  if(!p_hdr->m_cond_recv.timed_wait(lock, abs_time)){
                     if(p_hdr->is_empty())
                        return false;
                     break;
                  }
               }
               while (p_hdr->is_empty());
            break;

            //Paranoia check
            default:
            break;
         }
      }

      //There is at least one message ready to pick, get the top one
      ipcdetail::msg_hdr_t<VoidPointer> &top_msg = p_hdr->top_msg();

      //Get data from the message
      recvd_size     = top_msg.len;
      priority       = top_msg.priority;

      //Some cleanup to ease debugging
      top_msg.len       = 0;
      top_msg.priority  = 0;

      //Copy data to receiver's bufers
      std::memcpy(buffer, top_msg.data(), recvd_size);

      was_full = p_hdr->is_full();

      //Free top message and put it in the free message list
      p_hdr->free_top_msg();
   }  //Lock end

   //Notify outside lock to avoid contention. This might produce some
   //spurious wakeups, but it's usually far better than notifying inside.
   //If this reception changes the queue full state, notify senders
   if (was_full){
      p_hdr->m_cond_send.notify_one();
   }

   return true;
}

template<class VoidPointer>
inline typename windows_message_queue_t<VoidPointer>::size_type windows_message_queue_t<VoidPointer>::get_max_msg() const
{
   ipcdetail::mq_hdr_t<VoidPointer> *p_hdr = static_cast<ipcdetail::mq_hdr_t<VoidPointer>*>(m_shmem.get_user_address());
   return p_hdr ? p_hdr->m_max_num_msg : 0;  }

template<class VoidPointer>
inline typename windows_message_queue_t<VoidPointer>::size_type windows_message_queue_t<VoidPointer>::get_max_msg_size() const
{
   ipcdetail::mq_hdr_t<VoidPointer> *p_hdr = static_cast<ipcdetail::mq_hdr_t<VoidPointer>*>(m_shmem.get_user_address());
   return p_hdr ? p_hdr->m_max_msg_size : 0;
}

template<class VoidPointer>
inline typename windows_message_queue_t<VoidPointer>::size_type windows_message_queue_t<VoidPointer>::get_num_msg()
{
   ipcdetail::mq_hdr_t<VoidPointer> *p_hdr = static_cast<ipcdetail::mq_hdr_t<VoidPointer>*>(m_shmem.get_user_address());
   if(p_hdr){
      //---------------------------------------------
      scoped_lock<interprocess_mutex> lock(p_hdr->m_mutex);
      //---------------------------------------------
      return p_hdr->m_cur_num_msg;
   }

   return 0;
}

template<class VoidPointer>
inline bool windows_message_queue_t<VoidPointer>::remove(const char *name)
{  return true;  }

typedef windows_message_queue_t<offset_ptr<void> > windows_message_queue; 

/// @endcond

}} //namespace boost{  namespace interprocess{

#include <boost/interprocess/detail/config_end.hpp>

#endif   //#ifndef BOOST_INTERPROCESS_WINDOWS_MESSAGE_QUEUE_HPP
