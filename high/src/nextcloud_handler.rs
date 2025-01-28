use std::env::var;
use base64::prelude::*;

pub struct NextcloudHandler {
    nextcloud_user: String,
    nextcloud_pass: String,
    nextcloud_domain: String,
    nextcloud_chat_id: String,
}

impl NextcloudHandler {
    pub fn new() -> NextcloudHandler {
        let nextcloud_user = var("NEXTCLOUD_USER")
            .expect("NEXTCLOUD_USER env var missing");
        let nextcloud_pass = var("NEXTCLOUD_PASS")
            .expect("NEXTCLOUD_PASS env var missing");
        let nextcloud_domain = var("NEXTCLOUD_DOMAIN")
            .expect("NEXTCLOUD_DOMAIN env var missing");
        let nextcloud_chat_id = var("NEXTCLOUD_CHAT_ID")
            .expect("NEXTCLOUD_CHAT_ID env var missing");

        return NextcloudHandler {
            nextcloud_user,
            nextcloud_pass,
            nextcloud_domain,
            nextcloud_chat_id,
        };
    }

    pub fn send_message(&self, message: &'static str) {
        let client = reqwest::blocking::Client::new();

        let url = format!(
            "{}/ocs/v2.php/apps/spreed/api/v1/chat/{}",
            self.nextcloud_domain, self.nextcloud_chat_id
        );

        let authorization_creds = format!("{}:{}", self.nextcloud_user, self.nextcloud_pass);
        let authorization_base64 = BASE64_STANDARD.encode(authorization_creds);
        let authorization_header = format!("Basic {}", authorization_base64);

        let body = format!(
            "{{\n\
                \"token\": \"{}\",\n\
                \"message\": \"{}\"\n\
            }}",
            self.nextcloud_chat_id, message
        );

        client
            .post(url)
            .header("Content-Type", "application/json")
            .header("Accept", "application/json")
            .header("OCS-APIRequest", "true")
            .header("Authorization", authorization_header)
            .body(body)
            .send()
            .unwrap();
    }
}
