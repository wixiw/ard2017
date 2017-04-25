#include "0_Strategy2017.h"

using namespace ard;

Strategy2017::Strategy2017(Robot2017* _robot, String const& name):
        Action2017(_robot, name),
        lsaId(0),
        lsaStatus(NoLsa),
        lsa1(_robot, Monocolor)
{
    ASSERT_TEXT(_robot, "You did not attached a robot.");
    INIT_TABLE_TO_ZERO(actions);
    actions[0] = &lsa1;
}

LSA& Strategy2017::getLSA(eLsaId lsaId)
{
    ASSERT(actions[lsaId]);
    return *actions[lsaId];
}

void Strategy2017::update(DelayMs sinceLastCall)
{
    ASSERT(actions[lsaId]);
    if( lsaStatus == InProgress)
        actions[lsaId]->update(sinceLastCall);
    lsaStatus = actions[lsaId]->isFinished();
}
