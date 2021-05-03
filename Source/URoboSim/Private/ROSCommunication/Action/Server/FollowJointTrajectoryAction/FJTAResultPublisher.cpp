#include "ROSCommunication/Action/Server/FollowJointTrajectoryAction/FJTAResultPublisher.h"
#include "control_msgs/FollowJointTrajectoryActionResult.h"
#include "control_msgs/FollowJointTrajectoryResult.h"

DEFINE_LOG_CATEGORY_STATIC(LogRFJTAResultPublisher, Log, All);

URFJTAResultPublisher::URFJTAResultPublisher()
{
  MessageType = TEXT("control_msgs/FollowJointTrajectoryActionResult");
  FrameId = TEXT("odom");
}

void URFJTAResultPublisher::Init()
{
  Super::Init();
  JointController = Cast<URJointController>(Controller);
  if (JointController)
  {
    JointController->ActionFinished.AddDynamic(this, &URFJTAResultPublisher::PublishResult);
  }
}

void URFJTAResultPublisher::PublishResult(FGoalStatusInfo InStatusInfo)
{
  // if(Owner->bPublishResult)
  if (GetOwner() && JointController)
  {
    static int Seq = 0;

    UE_LOG(LogTemp, Error, TEXT("Publish Result FollowJointTrajectory"));
    TSharedPtr<control_msgs::FollowJointTrajectoryActionResult> ActionResult =
        MakeShareable(new control_msgs::FollowJointTrajectoryActionResult());

    ActionResult->SetHeader(std_msgs::Header(Seq, FROSTime(), FrameId));

    // uint8 status = Owner->Status;
    actionlib_msgs::GoalStatus GS(actionlib_msgs::GoalID(FROSTime(InStatusInfo.Secs, InStatusInfo.NSecs), InStatusInfo.Id), InStatusInfo.Status, "");
    ActionResult->SetStatus(GS);

    control_msgs::FollowJointTrajectoryResult Result(0);
    ActionResult->SetResult(Result);

    Handler->PublishMsg(Topic, ActionResult);
    Handler->Process();

    Seq++;

    // Owner->GoalStatusList.RemoveSingle(Status);
    UE_LOG(LogTemp, Error, TEXT("Publish finished"));

    // while(IndexArray.Num() != 0)
    //   {
    //     Index = IndexArray.Pop();
    //     Owner->GoalStatusList.RemoveAt(Index);
    //   }
    // for(auto& i : IndexArray)
    //   {
    //     Owner->GoalStatusList.RemoveAt(i);
    //   }
  }
}