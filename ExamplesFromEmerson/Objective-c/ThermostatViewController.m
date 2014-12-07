/* EXAMPLES
 //Set Schedule Mode
 [self setScheduleMode:YES];
 [self setScheduleMode:NO];
 
 //Continuous Back light example
 [self setSetting:@"ContinuousBacklight" value:@(YES)];
 [self setSetting:@"ContinuousBacklight" value:@(NO)];
 
 //LocalHumidityDisplay example
 [self setSetting:@"LocalHumidityDisplay" value:@(YES)];
 [self setSetting:@"LocalHumidityDisplay" value:@(NO)];
 
 //LocalTimeDisplay example
 [self setSetting:@"LocalTimeDisplay" value:@(YES)];
 [self setSetting:@"LocalTimeDisplay" value:@(NO)];
 
 
 //Set temp exmaples
 [self setHeatpointTemperature:[NSNumber numberWithInteger: 76]  units:ThermostatTemperatureUnits_Farenheight];
 [self setCoolpointTemperature:[NSNumber numberWithInteger: 71]  units:ThermostatTemperatureUnits_Farenheight];
 [self setAutoHeatpointTemperature:[NSNumber numberWithInteger:70]  units:ThermostatTemperatureUnits_Farenheight];
 [self setAutoCoolpointTemperature:[NSNumber numberWithInteger:70]  units:ThermostatTemperatureUnits_Farenheight];
 
 //Set Fan Mode Examples
 [self setFanMode:@"On"];
 [self setFanMode:@"Smart"];
 [self setFanMode:@"Auto"];
 
 //Set System Mode
 [self setSystemSettingSwitch:@"Heat"];
 [self setSystemSettingSwitch:@"Cool"];
 [self setSystemSettingSwitch:@"Aux"];
 [self setSystemSettingSwitch:@"Auto"];
 [self setSystemSettingSwitch:@"Off"];
 
 //Set hold mode
 [self setHoldMode:@"Off"];
 [self setHoldMode:@"Temporary"];
 [self setHoldMode:@"Permanant"];
 */

#import "ThermostatViewController.h"
#import "ThermostatEnums.h"

@interface ThermostatViewController ()
{

}

@property (nonatomic, strong) NSString* authorizationToken;

@end

@implementation ThermostatViewController

@synthesize messageField, messageTable;

- (void)awakeFromNib
{
    [super awakeFromNib];
}

#pragma mark -
#pragma mark View lifecycle

-(NSString*)thermostatID {
#warning PUT YOUR THERMOSTAT ID HERE
   return @"34 6f 92 00 B2 B4";

}

- (void)viewDidDisappear:(BOOL)animated
{
    [connection stop];
    hub = nil;
    connection.delegate = nil;
    connection = nil;

    [super viewDidDisappear:animated];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    NSString *urlString = [NSString stringWithFormat:@"http://bussrvstg.sensicomfort.com/api/authorize"];
    NSURL* url = [NSURL URLWithString:[urlString stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]];
    
    NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:30.0];
    [request setHTTPMethod:@"POST"];
    [request setValue:@"application/json; version=1" forHTTPHeaderField:@"Accept"];
    [request setValue:@"application/json; charset=utf-8" forHTTPHeaderField:@"Content-Type"];
    
#warning PUT YOUR USERNAME/PASSWORD HERE
    [request setHTTPBody:[NSJSONSerialization dataWithJSONObject:@{ @"UserName" : @"team19@globalhack.com", @"Password": @"globalhack" } options:0 error:nil]];

    [NSURLConnection sendAsynchronousRequest:request
                                       queue:[NSOperationQueue mainQueue]
                           completionHandler:^(NSURLResponse *response, NSData *data, NSError *error) {
                               id json = [NSJSONSerialization JSONObjectWithData:data options:0 error:nil];
                               self.authorizationToken = json[@"Token"];
                               [self connect];
                               
                           }];
}


#pragma mark -
#pragma mark View Actions

- (void)connect
{

    connection = [[SRHubConnection alloc] initWithURLString:@"http://bussrvstg.sensicomfort.com/realtime/" useDefault:NO];
    [connection addValue:self.authorizationToken forHTTPHeaderField:@"Authorization"];
    
    hub = [connection createHubProxy:@"thermostat-v1"];
    [hub on:@"online" perform:self selector:@selector(online:when:)];
    [hub on:@"update" perform:self selector:@selector(update:when:)];
    [hub on:@"offline" perform:self selector:@selector(offline:when:)];
    [hub on:@"error" perform:self selector:@selector(error:when:)];
    
    [connection setDelegate:self];
    [connection start];
    
    
    if(messagesReceived == nil)
    {
        messagesReceived = [[NSMutableArray alloc] init];
    }
    
}

-(IBAction)subscribeButtonPressed:(id)sender
{
    [hub invoke:@"Subscribe" withArgs:@[self.thermostatID] completionHandler:^(id response) {
        
        
    }];
}

-(void)setScheduleMode:(BOOL)on{
    NSString* modeAsString = on ? @"on" : @"off";
    [hub invoke:@"SetScheduleMode" withArgs:@[[self thermostatID], modeAsString] completionHandler:^(id response) {
        NSLog(@"setScheduleMode response = %@", response);
    }];
}

-(void)setAutoCoolpointTemperature:(NSNumber*)temperature units:(ThermostatTemperatureUnits)units {
    [hub invoke:@"SetAutoCool" withArgs:@[[self thermostatID], temperature, units == ThermostatTemperatureUnits_Farenheight ? @"F" : @"C"] completionHandler:^(id response) {
        NSLog(@"SetAutoCool response = %@", response);
    }];
}

-(void)setAutoHeatpointTemperature:(NSNumber*)temperature units:(ThermostatTemperatureUnits)units {
    [hub invoke:@"SetAutoHeat" withArgs:@[[self thermostatID], temperature, units == ThermostatTemperatureUnits_Farenheight ? @"F" : @"C"] completionHandler:^(id response) {
        NSLog(@"SetAutoHeat response = %@", response);
    }];
}

-(void)setCoolpointTemperature:(NSNumber*)temperature units:(ThermostatTemperatureUnits)units{
    [hub invoke:@"SetCool" withArgs:@[[self thermostatID], temperature, units == ThermostatTemperatureUnits_Farenheight ? @"F" : @"C"] completionHandler:^(id response) {
        NSLog(@"SetCool response = %@", response);
    }];
}

-(void)setHeatpointTemperature:(NSNumber*)temperature units:(ThermostatTemperatureUnits)units  {
    [hub invoke:@"SetHeat" withArgs:@[[self thermostatID], temperature, units == ThermostatTemperatureUnits_Farenheight ? @"F" : @"C"] completionHandler:^(id response) {
        NSLog(@"SetHeat response = %@", response);
    }];
}

-(void)setSystemSettingSwitch:(NSString*)mode {
    
    NSMutableArray *arguments = [NSMutableArray arrayWithArray:@[[self thermostatID]]];
    [arguments addObject:mode];
    
    [hub invoke:@"SetSystemMode" withArgs:arguments completionHandler:^(id response) {
        NSLog(@"SetSystemMode response = %@", response);
    }];
}

-(void)setFanMode:(NSString*)mode {
    
    NSMutableArray *arguments = [NSMutableArray arrayWithArray:@[[self thermostatID]]];
    [arguments addObject:mode];
    
    [hub invoke:@"SetFanMode" withArgs:arguments completionHandler:^(id response) {
        NSLog(@"SetFanMode response = %@", response);
    }];
}

-(void)setHoldMode:(NSString*)mode {
    
    NSMutableArray *arguments = [NSMutableArray arrayWithArray:@[[self thermostatID]]];
    [arguments addObject:mode];
    
    [hub invoke:@"SetHoldMode" withArgs:arguments completionHandler:^(id response) {
        NSLog(@"SetHoldMode response = %@", response);
    }];
}

-(void)setSetting:(NSString*)name value:(id)value {
    
    NSMutableArray *arguments = [NSMutableArray arrayWithArray:@[[self thermostatID]]];
    NSString* stringValue = nil;
    if ([value isKindOfClass:NSString.class]) {
        stringValue = value;
    } else if (!strcmp([value objCType], @encode(BOOL))) {
        stringValue = [value boolValue] ? @"on" : @"off";
    } else if ([value isKindOfClass:NSNumber.class]) {
        stringValue = [value stringValue];
    }
    [arguments addObject:name];
    [arguments addObject:stringValue];
    
    [hub invoke:@"ChangeSetting" withArgs:arguments completionHandler:^(id response) {
        NSLog(@"ChangeSetting response = %@", response);
    }];
}

#pragma mark -
#pragma mark Online Update Offline Error

- (void)online:(NSString *)id when:(NSString *)when
{
    if([id isEqualToString:connection.connectionId])
    {
        [messagesReceived insertObject:[NSString stringWithFormat:@"online at: %@",when] atIndex:0];
    }
    else
    {
        [messagesReceived insertObject:[NSString stringWithFormat:@"%@ online at: %@",id,when] atIndex:0];
    }
    NSLog(@"online date = %@", when);

    [self.messageTable reloadData];
}

- (void)update:(NSString *)id when:(NSString *)when
{
    [messagesReceived insertObject:[NSString stringWithFormat:@"update at: %@",when] atIndex:0];
    NSLog(@"Update date = %@", when);
    [self.messageTable reloadData];
}

- (void)offline:(NSString *)id when:(NSString *)when
{
    [messagesReceived insertObject:[NSString stringWithFormat:@"%@ offline at: %@",id,when] atIndex:0];
    [self.messageTable reloadData];
}

- (void)error:(NSString *)id when:(NSString *)when
{
    [messagesReceived insertObject:[NSString stringWithFormat:@"%@ error at: %@",id,when] atIndex:0];
    [self.messageTable reloadData];
}

#pragma mark -
#pragma mark SRConnection Delegate

- (void)SRConnectionDidOpen:(SRConnection *)connection
{
    [messagesReceived insertObject:@"Connection Opened" atIndex:0];
    [messageTable reloadData];
}

- (void)SRConnection:(SRConnection *)connection didReceiveData:(id)data
{
    NSLog(@"didReceiveData = %@", data);
   
}

- (void)SRConnectionDidClose:(SRConnection *)connection
{
    [messagesReceived insertObject:@"Connection Closed" atIndex:0];
    [messageTable reloadData];
}

- (void)SRConnection:(SRConnection *)connection didReceiveError:(NSError *)error
{
    [messagesReceived insertObject:[NSString stringWithFormat:@"Connection Error: %@",error.localizedDescription] atIndex:0];
    [messageTable reloadData];
}


#pragma mark -
#pragma mark TableView datasource

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [messagesReceived count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Cell";
    
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier];
    }
    
    cell.textLabel.text = messagesReceived[indexPath.row];
    
    return cell;
}

@end
