; ModuleID = 'batchmaker_test2.c'
source_filename = "batchmaker_test2.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.packet = type { i32, i32, i32, i32 }

@.str = private unnamed_addr constant [10 x i8] c"batch_arg\00", section "llvm.metadata"
@.str.1 = private unnamed_addr constant [19 x i8] c"batchmaker_test2.c\00", section "llvm.metadata"
@.str.2 = private unnamed_addr constant [16 x i8] c"packet ip = %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [16 x i8] c"packet id = %d\0A\00", align 1
@.str.4 = private unnamed_addr constant [16 x i8] c"tas_batch_maker\00", section "llvm.metadata"
@llvm.global.annotations = appending global [1 x { i8*, i8*, i8*, i32 }] [{ i8*, i8*, i8*, i32 } { i8* bitcast (i32 (%struct.packet*)* @process_packet to i8*), i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.4, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 17 }], section "llvm.metadata"

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @process_packet(%struct.packet*) #0 {
  %2 = alloca %struct.packet*, align 8
  %3 = alloca %struct.packet*, align 8
  store %struct.packet* %0, %struct.packet** %2, align 8
  %4 = bitcast %struct.packet** %2 to i8*
  call void @llvm.var.annotation(i8* %4, i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str, i32 0, i32 0), i8* getelementptr inbounds ([19 x i8], [19 x i8]* @.str.1, i32 0, i32 0), i32 17)
  %5 = load %struct.packet*, %struct.packet** %2, align 8
  store %struct.packet* %5, %struct.packet** %3, align 8
  %6 = load %struct.packet*, %struct.packet** %3, align 8
  %7 = getelementptr inbounds %struct.packet, %struct.packet* %6, i32 0, i32 0
  %8 = load i32, i32* %7, align 4
  %9 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.2, i32 0, i32 0), i32 %8)
  %10 = load %struct.packet*, %struct.packet** %3, align 8
  %11 = getelementptr inbounds %struct.packet, %struct.packet* %10, i32 0, i32 1
  %12 = load i32, i32* %11, align 4
  %13 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([16 x i8], [16 x i8]* @.str.3, i32 0, i32 0), i32 %12)
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.var.annotation(i8*, i8*, i8*, i32) #1

declare i32 @printf(i8*, ...) #2

; Function Attrs: noinline nounwind optnone sspstrong uwtable
define dso_local i32 @caller_fn() #0 {
  %1 = alloca %struct.packet*, align 8
  %2 = call noalias i8* @malloc(i64 16) #1
  %3 = bitcast i8* %2 to %struct.packet*
  store %struct.packet* %3, %struct.packet** %1, align 8
  %4 = load %struct.packet*, %struct.packet** %1, align 8
  %5 = getelementptr inbounds %struct.packet, %struct.packet* %4, i32 0, i32 0
  store i32 1000, i32* %5, align 4
  %6 = load %struct.packet*, %struct.packet** %1, align 8
  %7 = getelementptr inbounds %struct.packet, %struct.packet* %6, i32 0, i32 1
  store i32 1, i32* %7, align 4
  %8 = load %struct.packet*, %struct.packet** %1, align 8
  %9 = getelementptr inbounds %struct.packet, %struct.packet* %8, i32 0, i32 2
  store i32 200, i32* %9, align 4
  %10 = load %struct.packet*, %struct.packet** %1, align 8
  %11 = getelementptr inbounds %struct.packet, %struct.packet* %10, i32 0, i32 3
  store i32 300, i32* %11, align 4
  %12 = load %struct.packet*, %struct.packet** %1, align 8
  %13 = call i32 @process_packet(%struct.packet* %12)
  %14 = load %struct.packet*, %struct.packet** %1, align 8
  %15 = bitcast %struct.packet* %14 to i8*
  call void @free(i8* %15) #1
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #3

; Function Attrs: nounwind
declare void @free(i8*) #3

attributes #0 = { noinline nounwind optnone sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.1 (tags/RELEASE_801/final)"}
