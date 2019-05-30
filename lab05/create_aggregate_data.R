# prepare data
results = read.csv("times.csv",sep=";")
avg_results = aggregate( time_better ~ size, data=results, FUN=mean)
avg_results$time_naive = aggregate(time_naive ~ size,data = results,FUN = mean)$time_naive
avg_results$sd_time_better = aggregate(time_better ~ size,data=results,FUN = sd)$time_better
avg_results$sd_time_naive = aggregate(time_naive ~ size,data=results,FUN = sd)$time_naive

# make plot
ggplot(avg_results,aes(size)) + geom_point(aes(y = time_better,colour="darkblue")) + geom_point(aes(y = time_naive,colour="red")) + geom_errorbar(data=avg_results,mapping = aes(x=size,ymin=time_better-(sd_time_better/2),ymax=time_better+(sd_time_better/2))) + geom_errorbar(data=avg_results,mapping = aes(x=size,ymin=time_naive-(sd_time_naive/2),ymax=time_naive+(sd_time_naive/2))) + labs(title = "Times of multyplying", x = "Sizes", y ="Times")

fit = lm(time_better ~ poly(size, 3, raw=TRUE), data=avg_results)
newdata = data.frame(size = seq(20,380, length.out=380))
newdata$time_better = predict(fit, newdata)
last_plot() + geom_line(data=newdata,aes(size,time_better,colour="darkblue"))

fit2 = lm(time_naive ~ poly(size, 3, raw=TRUE), data=avg_results)
newdata$time_naive = predict(fit2, newdata)
last_plot() + geom_line(data=newdata,aes(size,time_naive,colour="red")) +scale_color_discrete(name = "Approximation", labels = c("time better", "time naive"))