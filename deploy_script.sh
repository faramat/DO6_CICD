echo "Deploy"
if scp ./src/cat/s21_cat ./src/grep/s21_grep ws2:~/ ; then
ssh ws2 "mv s21_cat s21_grep /usr/local/bin"
  echo -e "\e[32mArtifacts_copied\e[0m"
else
  echo -e "\e[31mNo_artifacts\e[0m"
exit 1
fi
