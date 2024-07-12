BOT_TOKEN="7384295865:AAFwnq9jn-GqxHh9E058tnz5M1BsbekAIe8"
CHAT_WITH_USER_ID="6690447927"

if [ "$CI_JOB_STATUS" == "success" ]; then
  MESSAGE="$CI_JOB_STAGE DONE"
else
  MESSAGE="$CI_JOB_STAGE FAILED"
fi

curl -s -X POST https://api.telegram.org/bot${BOT_TOKEN}/sendMessage -d chat_id=${CHAT_WITH_USER_ID} -d text="${MESSAGE}" -d parse_mode="html"



