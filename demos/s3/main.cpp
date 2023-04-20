#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/s3/S3Client.h>
#include <aws/s3/model/CreateBucketRequest.h>
#include <iostream>

using namespace Aws::S3;
using namespace Aws::S3::Model;
using namespace std;

int main(int argc, char *argv[]) {
    // 初始化API
    Aws::SDKOptions options;
    options.loggingOptions.logLevel = Aws::Utils::Logging::LogLevel::Trace;
    Aws::InitAPI(options);

    Aws::Client::ClientConfiguration cfg;
    cfg.endpointOverride = "10.16.213.12:81";// S3服务器地址和端口
    cfg.scheme = Aws::Http::Scheme::HTTP;
    cfg.verifySSL = false;

    Aws::Auth::AWSCredentials cred("31A4EB4FA9E958A3D304", "WzA0QTYxQ0VDQ0FBQzA4MzA0NjEwMzhDOTkzQzQw");// ak,sk
    S3Client client(cred, nullptr, cfg);

    auto response = client.ListBuckets();
    if (response.IsSuccess()) {
        auto buckets = response.GetResult().GetBuckets();
        for (auto & bucket : buckets) {
            cout << bucket.GetName() << "\t" << bucket.GetCreationDate().ToLocalTimeString(Aws::Utils::DateFormat::ISO_8601) << endl;
        }
    } else {
        cout << "Error while ListBuckets " << response.GetError().GetExceptionName()
             << " " << response.GetError().GetMessage() << endl;
    }

    Aws::ShutdownAPI(options);
    return 0;
}