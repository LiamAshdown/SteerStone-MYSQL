#include "DatabaseWorker.h"
#include "Operator.h"

namespace SteerStone
{
    /// Constructor
    DatabaseWorker::DatabaseWorker(std::queue<Operator*>* p_Queue) :
        m_Queue(p_Queue)
    {
        m_WorkerThread = std::thread(&DatabaseWorker::WorkerThread, this);
    }

    /// Deconstructor
    DatabaseWorker::~DatabaseWorker()
    {
    }

    /// WorkerThread
    /// Executes asynchronous queries
    void DatabaseWorker::WorkerThread()
    {
        while (true)
        {
            if (m_Queue->size() > 0)
            {
                Operator* l_Operator = m_Queue->front();
                l_Operator->Execute();

                m_Queue->pop();

                delete l_Operator;
            }
        }
    }
} ///< NAMESPACE STEERSTONE