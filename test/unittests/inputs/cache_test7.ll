; ModuleID = '<stdin>'
source_filename = "cache_test7.c"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

%struct.packet = type { i64, i64, i64, i64 }

; Function Attrs: noinline nounwind sspstrong uwtable
define dso_local void @test_fn(%struct.packet**) #0 {
  br label %2

; <label>:2:                                      ; preds = %10, %1
  %indvars.iv = phi i64 [ %indvars.iv.next, %10 ], [ 0, %1 ]
  %exitcond = icmp ne i64 %indvars.iv, 2
  br i1 %exitcond, label %3, label %11

; <label>:3:                                      ; preds = %2
  %4 = getelementptr inbounds %struct.packet*, %struct.packet** %0, i64 %indvars.iv
  %5 = load %struct.packet*, %struct.packet** %4, align 8
  %6 = getelementptr inbounds %struct.packet, %struct.packet* %5, i32 0, i32 3
  store i64 1, i64* %6, align 8
  %7 = getelementptr inbounds %struct.packet, %struct.packet* %5, i32 0, i32 2
  store i64 2, i64* %7, align 8
  %8 = getelementptr inbounds %struct.packet, %struct.packet* %5, i32 0, i32 1
  store i64 3, i64* %8, align 8
  %9 = getelementptr inbounds %struct.packet, %struct.packet* %5, i32 0, i32 0
  store i64 4, i64* %9, align 8
  br label %10

; <label>:10:                                     ; preds = %3
  %indvars.iv.next = add nuw nsw i64 %indvars.iv, 1
  br label %2

; <label>:11:                                     ; preds = %2
  ret void
}

; Function Attrs: noinline nounwind sspstrong uwtable
define dso_local i32 @caller_fn() #0 {
  %1 = alloca [2 x %struct.packet*], align 16
  %2 = call noalias i8* @malloc(i64 32) #2
  %3 = bitcast i8* %2 to %struct.packet*
  %4 = call noalias i8* @malloc(i64 32) #2
  %5 = bitcast i8* %4 to %struct.packet*
  %6 = getelementptr inbounds [2 x %struct.packet*], [2 x %struct.packet*]* %1, i64 0, i64 0
  store %struct.packet* %3, %struct.packet** %6, align 8
  %7 = getelementptr inbounds %struct.packet*, %struct.packet** %6, i64 1
  store %struct.packet* %5, %struct.packet** %7, align 8
  %8 = getelementptr inbounds [2 x %struct.packet*], [2 x %struct.packet*]* %1, i32 0, i32 0
  call void @test_fn(%struct.packet** %8)
  %9 = bitcast %struct.packet* %3 to i8*
  call void @free(i8* %9) #2
  %10 = bitcast %struct.packet* %5 to i8*
  call void @free(i8* %10) #2
  ret i32 0
}

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #1

; Function Attrs: nounwind
declare void @free(i8*) #1

; Function Attrs: noinline nounwind sspstrong uwtable
define dso_local i32 @main() #0 {
  %1 = call i32 @caller_fn()
  ret i32 0
}

attributes #0 = { noinline nounwind sspstrong uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{i32 7, !"PIE Level", i32 2}
!3 = !{!"clang version 8.0.0 (tags/RELEASE_800/final)"}
