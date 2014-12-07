#import <UIKit/UIKit.h>
#import "SignalR.h"

@interface ThermostatViewController : UIViewController <SRConnectionDelegate>
{
    SRHubConnection *connection;
    SRHubProxy *hub;
    NSMutableArray *messagesReceived;
}

@property (nonatomic, strong) IBOutlet UITableView *messageTable;
@property (nonatomic, strong) IBOutlet UITextField *messageField;

-(IBAction)subscribeButtonPressed:(id)sender;
@end
