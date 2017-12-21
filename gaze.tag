<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="class">
    <name>Events</name>
    <filename>class_events.html</filename>
  </compound>
  <compound kind="namespace">
    <name>eyeLike</name>
    <filename>namespaceeye_like.html</filename>
  </compound>
  <compound kind="namespace">
    <name>gaze</name>
    <filename>namespacegaze.html</filename>
    <namespace>gaze::pipeline</namespace>
    <namespace>gaze::util</namespace>
    <class kind="class">gaze::GazeTracker</class>
    <class kind="class">gaze::Pipeline</class>
    <class kind="class">gaze::PipelineStep</class>
  </compound>
  <compound kind="class">
    <name>gaze::GazeTracker</name>
    <filename>classgaze_1_1_gaze_tracker.html</filename>
    <member kind="function">
      <type></type>
      <name>GazeTracker</name>
      <anchorfile>classgaze_1_1_gaze_tracker.html</anchorfile>
      <anchor>a21e26f7e540e02c2d4775e6128dd0d58</anchor>
      <arglist>(const std::string subject_id=&quot;default_subject&quot;, const bool debug=false)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>calibrate</name>
      <anchorfile>classgaze_1_1_gaze_tracker.html</anchorfile>
      <anchor>a011cd63e47d9bc94d1e360c8bd9aa08e</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>const std::pair&lt; int, int &gt;</type>
      <name>get_current_gaze_point</name>
      <anchorfile>classgaze_1_1_gaze_tracker.html</anchorfile>
      <anchor>a03913ac81393023fc991d8e877807b87</anchor>
      <arglist>() const </arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>init</name>
      <anchorfile>classgaze_1_1_gaze_tracker.html</anchorfile>
      <anchor>a5a39d281355fce42f0308ac6d191df13</anchor>
      <arglist>(const std::string subject_id, const bool debug=false)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>start_trial</name>
      <anchorfile>classgaze_1_1_gaze_tracker.html</anchorfile>
      <anchor>afba9342de76358c41f999eb77e4709bd</anchor>
      <arglist>(const std::string identifier)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stop_trial</name>
      <anchorfile>classgaze_1_1_gaze_tracker.html</anchorfile>
      <anchor>a44b33bd80c25dab5d89388352b8ee8e8</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>gaze::Pipeline</name>
    <filename>classgaze_1_1_pipeline.html</filename>
    <member kind="function">
      <type></type>
      <name>Pipeline</name>
      <anchorfile>classgaze_1_1_pipeline.html</anchorfile>
      <anchor>acc35cb4a5e9189ea71ea1fa85c3ad229</anchor>
      <arglist>(std::vector&lt; PipelineStep * &gt; steps, const bool start=true)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>~Pipeline</name>
      <anchorfile>classgaze_1_1_pipeline.html</anchorfile>
      <anchor>a47d76d2ebc4dcaa1320fea0546c5a3f2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>start</name>
      <anchorfile>classgaze_1_1_pipeline.html</anchorfile>
      <anchor>a3c41315291e9c7d9217c2b4ea73d6527</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>stop</name>
      <anchorfile>classgaze_1_1_pipeline.html</anchorfile>
      <anchor>a75c98fd270ad8d2faeb1e5d19602c8a6</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>util::Data</type>
      <name>get_data</name>
      <anchorfile>classgaze_1_1_pipeline.html</anchorfile>
      <anchor>a87fe369b8d2e938cd381e16e29906b61</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; PipelineStep * &gt;</type>
      <name>get_steps</name>
      <anchorfile>classgaze_1_1_pipeline.html</anchorfile>
      <anchor>ab2b07c6d2343a264fddb73168bb86a8f</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>gaze::PipelineStep</name>
    <filename>classgaze_1_1_pipeline_step.html</filename>
    <member kind="function">
      <type></type>
      <name>PipelineStep</name>
      <anchorfile>classgaze_1_1_pipeline_step.html</anchorfile>
      <anchor>ad1c0a5fc7005acb129c519e045a705a2</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>get_name</name>
      <anchorfile>classgaze_1_1_pipeline_step.html</anchorfile>
      <anchor>aca11425a364e3bf7d43468fb465d08f1</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~PipelineStep</name>
      <anchorfile>classgaze_1_1_pipeline_step.html</anchorfile>
      <anchor>a8e509d7ed87e6a2e0b84cbd84fac8e50</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>process</name>
      <anchorfile>classgaze_1_1_pipeline_step.html</anchorfile>
      <anchor>a98ea605fed6983188c8af8d222908b59</anchor>
      <arglist>(util::Data &amp;data)=0</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>int</type>
      <name>number</name>
      <anchorfile>classgaze_1_1_pipeline_step.html</anchorfile>
      <anchor>a40e0c764e4067f769bfd9f3c2f8cb61f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::string</type>
      <name>name</name>
      <anchorfile>classgaze_1_1_pipeline_step.html</anchorfile>
      <anchor>ac87249a515fa0cd433197f300e9300b0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>gaze::gui::DebugWindow</name>
    <filename>classgaze_1_1gui_1_1_debug_window.html</filename>
    <member kind="function">
      <type>void</type>
      <name>process_data</name>
      <anchorfile>classgaze_1_1gui_1_1_debug_window.html</anchorfile>
      <anchor>af5d8fb1bc05dba2f0c2bd8c32b3b0d4a</anchor>
      <arglist>(int idx=-1)</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>DebugWindow</name>
      <anchorfile>classgaze_1_1gui_1_1_debug_window.html</anchorfile>
      <anchor>ac15dac672b6a21dc6282d6b74e5fc9aa</anchor>
      <arglist>(Pipeline *pipeline)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>on_user_event</name>
      <anchorfile>classgaze_1_1gui_1_1_debug_window.html</anchorfile>
      <anchor>a5e3670910c9ba175a3e6212a9de7c358</anchor>
      <arglist>(void *event_data, int event_type) override</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>gaze::gui::EventManager</name>
    <filename>classgaze_1_1gui_1_1_event_manager.html</filename>
    <member kind="function">
      <type>void</type>
      <name>publish</name>
      <anchorfile>classgaze_1_1gui_1_1_event_manager.html</anchorfile>
      <anchor>a7270cb04d9f80413b210c15c21eaa238</anchor>
      <arglist>(void *data, Events event)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>subscribe</name>
      <anchorfile>classgaze_1_1gui_1_1_event_manager.html</anchorfile>
      <anchor>a8889fcbaf4859e64b22c34678bbecd47</anchor>
      <arglist>(dlib::base_window *subscriber)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>unsubscribe</name>
      <anchorfile>classgaze_1_1gui_1_1_event_manager.html</anchorfile>
      <anchor>ae0d53dea8ac41db042279693967fd909</anchor>
      <arglist>(dlib::base_window *subscriber)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static EventManager &amp;</type>
      <name>instance</name>
      <anchorfile>classgaze_1_1gui_1_1_event_manager.html</anchorfile>
      <anchor>aa07e978ef04983dcd610ae72257a8e38</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>gaze::gui::VisualizeableBase</name>
    <filename>classgaze_1_1gui_1_1_visualizeable_base.html</filename>
    <member kind="function" virtualness="virtual">
      <type>virtual</type>
      <name>~VisualizeableBase</name>
      <anchorfile>classgaze_1_1gui_1_1_visualizeable_base.html</anchorfile>
      <anchor>a55a2f51d8ec59d0cdaa2d1014c0b609d</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual std::shared_ptr&lt; dlib::drawable &gt;</type>
      <name>init</name>
      <anchorfile>classgaze_1_1gui_1_1_visualizeable_base.html</anchorfile>
      <anchor>ab492968932b526e09bae13a151585371</anchor>
      <arglist>(dlib::drawable_window &amp;parent, int width, int height, std::string text=&quot;No visualization.&quot;)=0</arglist>
    </member>
    <member kind="function" virtualness="virtual">
      <type>virtual void</type>
      <name>remove_widget</name>
      <anchorfile>classgaze_1_1gui_1_1_visualizeable_base.html</anchorfile>
      <anchor>a43edb15dbbccc3c391a30a3cd4966b20</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" virtualness="pure">
      <type>virtual void</type>
      <name>visualize</name>
      <anchorfile>classgaze_1_1gui_1_1_visualizeable_base.html</anchorfile>
      <anchor>a3ada293be1d674531f0a0f65221032aa</anchor>
      <arglist>(util::Data &amp;data)=0</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>gaze::gui::Visualizeable</name>
    <filename>classgaze_1_1gui_1_1_visualizeable.html</filename>
    <templarg></templarg>
    <base>gaze::gui::VisualizeableBase</base>
    <member kind="function">
      <type>std::shared_ptr&lt; dlib::drawable &gt;</type>
      <name>init</name>
      <anchorfile>classgaze_1_1gui_1_1_visualizeable.html</anchorfile>
      <anchor>a1536446f698fb413b020a43ca12a0876</anchor>
      <arglist>(dlib::drawable_window &amp;parent, int width, int height, std::string text=&quot;No visualization.&quot;) final</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>remove_widget</name>
      <anchorfile>classgaze_1_1gui_1_1_visualizeable.html</anchorfile>
      <anchor>a60e9c6c7ce98ef9f465ea9aeae3809b3</anchor>
      <arglist>() override</arglist>
    </member>
    <member kind="variable" protection="protected">
      <type>std::shared_ptr&lt; widget_type &gt;</type>
      <name>widget</name>
      <anchorfile>classgaze_1_1gui_1_1_visualizeable.html</anchorfile>
      <anchor>af881cd99f0aae7e92e4217cb191777e0</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>gaze::gui::traits::sizeable</name>
    <filename>structgaze_1_1gui_1_1traits_1_1sizeable.html</filename>
  </compound>
  <compound kind="struct">
    <name>gaze::gui::traits::textable</name>
    <filename>structgaze_1_1gui_1_1traits_1_1textable.html</filename>
  </compound>
  <compound kind="struct">
    <name>gaze::gui::traits::widget_trait</name>
    <filename>structgaze_1_1gui_1_1traits_1_1widget__trait.html</filename>
    <templarg></templarg>
  </compound>
  <compound kind="struct">
    <name>gaze::gui::traits::widget_trait&lt; dlib::label &gt;</name>
    <filename>structgaze_1_1gui_1_1traits_1_1widget__trait_3_01dlib_1_1label_01_4.html</filename>
    <member kind="typedef">
      <type>textable</type>
      <name>tag</name>
      <anchorfile>structgaze_1_1gui_1_1traits_1_1widget__trait_3_01dlib_1_1label_01_4.html</anchorfile>
      <anchor>af9b7489a059f2ad1e8093e40b31e3dfc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>gaze::gui::traits::widget_trait&lt; dlib::image_display &gt;</name>
    <filename>structgaze_1_1gui_1_1traits_1_1widget__trait_3_01dlib_1_1image__display_01_4.html</filename>
    <member kind="typedef">
      <type>sizeable</type>
      <name>tag</name>
      <anchorfile>structgaze_1_1gui_1_1traits_1_1widget__trait_3_01dlib_1_1image__display_01_4.html</anchorfile>
      <anchor>a837b91928c12117c4eee302fa3b03ca8</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>gaze::gui::traits::widget_trait&lt; dlib::perspective_display &gt;</name>
    <filename>structgaze_1_1gui_1_1traits_1_1widget__trait_3_01dlib_1_1perspective__display_01_4.html</filename>
    <member kind="typedef">
      <type>sizeable</type>
      <name>tag</name>
      <anchorfile>structgaze_1_1gui_1_1traits_1_1widget__trait_3_01dlib_1_1perspective__display_01_4.html</anchorfile>
      <anchor>a54c0a026a7452ce9a20e43a6218478c4</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>gaze::pipeline</name>
    <filename>namespacegaze_1_1pipeline.html</filename>
    <class kind="class">gaze::pipeline::EyeLike</class>
    <class kind="class">gaze::pipeline::FaceLandmarks</class>
    <class kind="class">gaze::pipeline::FallbackStep</class>
    <class kind="class">gaze::pipeline::GazePointCalculation</class>
    <class kind="class">gaze::pipeline::HeadPoseEstimation</class>
    <class kind="class">gaze::pipeline::PupilLocalization</class>
    <class kind="class">gaze::pipeline::SourceCapture</class>
  </compound>
  <compound kind="class">
    <name>gaze::pipeline::EyeLike</name>
    <filename>classgaze_1_1pipeline_1_1_eye_like.html</filename>
    <base>gaze::PipelineStep</base>
    <base>gaze::gui::Visualizeable</base>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classgaze_1_1pipeline_1_1_eye_like.html</anchorfile>
      <anchor>a49e133f95f5629b84ea59306d02cc08b</anchor>
      <arglist>(util::Data &amp;data) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>visualize</name>
      <anchorfile>classgaze_1_1pipeline_1_1_eye_like.html</anchorfile>
      <anchor>a6a8f37f89e7fd29f0ab0ff3384ea16aa</anchor>
      <arglist>(util::Data &amp;data) override</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>gaze::pipeline::FaceLandmarks</name>
    <filename>classgaze_1_1pipeline_1_1_face_landmarks.html</filename>
    <base>gaze::PipelineStep</base>
    <base>gaze::gui::Visualizeable</base>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classgaze_1_1pipeline_1_1_face_landmarks.html</anchorfile>
      <anchor>a4847e5df7d07904e825c3f10efe34980</anchor>
      <arglist>(util::Data &amp;data) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>visualize</name>
      <anchorfile>classgaze_1_1pipeline_1_1_face_landmarks.html</anchorfile>
      <anchor>a5bc91835d33b5002d582805235c2d0d2</anchor>
      <arglist>(util::Data &amp;data) override</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>gaze::pipeline::FallbackStep</name>
    <filename>classgaze_1_1pipeline_1_1_fallback_step.html</filename>
    <base>gaze::PipelineStep</base>
    <base>gaze::gui::Visualizeable</base>
    <member kind="function">
      <type></type>
      <name>FallbackStep</name>
      <anchorfile>classgaze_1_1pipeline_1_1_fallback_step.html</anchorfile>
      <anchor>a74c12e515acda61657198d496792684d</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classgaze_1_1pipeline_1_1_fallback_step.html</anchorfile>
      <anchor>a41a5917c0f33488bd8f5a0fb2b407b35</anchor>
      <arglist>(util::Data &amp;) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>visualize</name>
      <anchorfile>classgaze_1_1pipeline_1_1_fallback_step.html</anchorfile>
      <anchor>a2b59b4dcbc8d104a56fb4bb230f0191a</anchor>
      <arglist>(util::Data &amp;) override</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>gaze::pipeline::GazePointCalculation</name>
    <filename>classgaze_1_1pipeline_1_1_gaze_point_calculation.html</filename>
    <base>gaze::PipelineStep</base>
    <base>gaze::gui::Visualizeable</base>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classgaze_1_1pipeline_1_1_gaze_point_calculation.html</anchorfile>
      <anchor>a266cfdb3c307cd31beacd7d2357709ac</anchor>
      <arglist>(util::Data &amp;data) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>visualize</name>
      <anchorfile>classgaze_1_1pipeline_1_1_gaze_point_calculation.html</anchorfile>
      <anchor>acaffe412870ed499be707ead822486bb</anchor>
      <arglist>(util::Data &amp;data) override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>void</type>
      <name>set_sensor_size</name>
      <anchorfile>classgaze_1_1pipeline_1_1_gaze_point_calculation.html</anchorfile>
      <anchor>ae190813d2898f90dc168e641a0c68138</anchor>
      <arglist>(double sensor_diagonal, double aspect_ratio)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>gaze::pipeline::HeadPoseEstimation</name>
    <filename>classgaze_1_1pipeline_1_1_head_pose_estimation.html</filename>
    <base>gaze::PipelineStep</base>
    <base>gaze::gui::Visualizeable</base>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classgaze_1_1pipeline_1_1_head_pose_estimation.html</anchorfile>
      <anchor>ae707709628d06b3a8826dfa15176eb7c</anchor>
      <arglist>(util::Data &amp;data) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>visualize</name>
      <anchorfile>classgaze_1_1pipeline_1_1_head_pose_estimation.html</anchorfile>
      <anchor>a73dbd10b30be0d2f47ccd4d43a648a93</anchor>
      <arglist>(util::Data &amp;data) override</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>cv::Matx33d</type>
      <name>read_or_set_camera_matrix</name>
      <anchorfile>classgaze_1_1pipeline_1_1_head_pose_estimation.html</anchorfile>
      <anchor>a41b7b713e936c9a5153b8810e3804fb3</anchor>
      <arglist>(const util::Data &amp;data)</arglist>
    </member>
    <member kind="function" protection="protected">
      <type>cv::Mat</type>
      <name>get_and_maybe_read_distortions</name>
      <anchorfile>classgaze_1_1pipeline_1_1_head_pose_estimation.html</anchorfile>
      <anchor>af1631b6daa2c394812fefc63f8dba33a</anchor>
      <arglist>(const util::Data &amp;data)</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>gaze::pipeline::PupilLocalization</name>
    <filename>classgaze_1_1pipeline_1_1_pupil_localization.html</filename>
    <base>gaze::PipelineStep</base>
    <base>gaze::gui::Visualizeable</base>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classgaze_1_1pipeline_1_1_pupil_localization.html</anchorfile>
      <anchor>a3b74257d61a1a0773292c5ad94282cc0</anchor>
      <arglist>(util::Data &amp;data) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>visualize</name>
      <anchorfile>classgaze_1_1pipeline_1_1_pupil_localization.html</anchorfile>
      <anchor>a1f354722d1301027200264de93dbbb07</anchor>
      <arglist>(util::Data &amp;data) override</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>gaze::pipeline::SourceCapture</name>
    <filename>classgaze_1_1pipeline_1_1_source_capture.html</filename>
    <base>gaze::PipelineStep</base>
    <base>gaze::gui::Visualizeable</base>
    <member kind="function">
      <type>void</type>
      <name>process</name>
      <anchorfile>classgaze_1_1pipeline_1_1_source_capture.html</anchorfile>
      <anchor>a5a008007a14ac66b975a8f1f0e02c5c9</anchor>
      <arglist>(util::Data &amp;data) override</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>visualize</name>
      <anchorfile>classgaze_1_1pipeline_1_1_source_capture.html</anchorfile>
      <anchor>a8501a73d078a9b07c175acda76ed4cf5</anchor>
      <arglist>(util::Data &amp;data) override</arglist>
    </member>
    <member kind="function">
      <type></type>
      <name>SourceCapture</name>
      <anchorfile>classgaze_1_1pipeline_1_1_source_capture.html</anchorfile>
      <anchor>ab3eb1a760ed34b121050887b0f94e755</anchor>
      <arglist>()</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>gaze::util</name>
    <filename>namespacegaze_1_1util.html</filename>
    <class kind="struct">gaze::util::Data</class>
    <member kind="function">
      <type>dlib::rectangle</type>
      <name>get_eye_region</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>a1ebd13439848a4d9ac10d7e8dee7dc0e</anchor>
      <arglist>(int eye, dlib::full_object_detection object_detection)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>parse_aspect_ratio</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>a9420aace2ba5209448ba4822eaef94c8</anchor>
      <arglist>(std::string aspect_ratio_string)</arglist>
    </member>
    <member kind="function">
      <type>double</type>
      <name>clamp</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>a15dc5bd0f165141adfc7473fd9bcfe83</anchor>
      <arglist>(double value, double min, double max)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>fill_displacement_tables</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>a885f4db0683f4ae23bd922b9d6dbc20c</anchor>
      <arglist>(dlib::matrix&lt; double &gt; &amp;table_x, dlib::matrix&lt; double &gt; &amp;table_y, int size)</arglist>
    </member>
    <member kind="function">
      <type>YAML::Node</type>
      <name>get_config</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>a13f3893ed58ff0693776c7727129dadf</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>YAML::Node</type>
      <name>get_config</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>a78bea599eb39c90c5824a3589584ec09</anchor>
      <arglist>(int pipeline_step_number)</arglist>
    </member>
    <member kind="function">
      <type>dlib::point</type>
      <name>cv_to_dlib</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>a8bb31dd9ecd2a956736ef9139d413163</anchor>
      <arglist>(const cv::Point &amp;to_convert)</arglist>
    </member>
    <member kind="function">
      <type>dlib::rectangle</type>
      <name>cv_to_dlib</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>abf8749a1df5cb54d7d0450bc1bce7b7b</anchor>
      <arglist>(const cv::Rect &amp;to_convert)</arglist>
    </member>
    <member kind="function">
      <type>cv::Rect</type>
      <name>dlib_to_cv</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>a3a3a294e5f20904c54676bbde3c606f3</anchor>
      <arglist>(const dlib::rectangle &amp;to_convert)</arglist>
    </member>
    <member kind="function">
      <type>std::vector&lt; dlib::chip_details &gt;</type>
      <name>get_eyes_chip_details</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>af5a6dafca98936bb6a798476b61a1ced</anchor>
      <arglist>(const dlib::full_object_detection object_detection)</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>normalize_and_threshold_gradients</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>a3b9c6818278203ec0f56a66513e89876</anchor>
      <arglist>(dlib::matrix&lt; T &gt; &amp;horizontal, dlib::matrix&lt; T &gt; &amp;vertical, double relative_threshold=-1)</arglist>
    </member>
    <member kind="function">
      <type>std::ostream &amp;</type>
      <name>operator&lt;&lt;</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>a09cb2e917c4d30fa24862b41abf6851f</anchor>
      <arglist>(std::ostream &amp;ostr, const Data &amp;data)</arglist>
    </member>
    <member kind="function">
      <type>dlib::point</type>
      <name>cv_to_dlib</name>
      <anchorfile>namespacegaze_1_1util.html</anchorfile>
      <anchor>a51a246c53cfc1bc40ad2895f6673d879</anchor>
      <arglist>(const cv::Vec3d &amp;to_convert)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>gaze::util::Data</name>
    <filename>structgaze_1_1util_1_1_data.html</filename>
    <member kind="function">
      <type>Data &amp;</type>
      <name>operator=</name>
      <anchorfile>structgaze_1_1util_1_1_data.html</anchorfile>
      <anchor>aaa9cf84b89acbc292f5de22f5eea1424</anchor>
      <arglist>(Data data)</arglist>
    </member>
    <member kind="variable">
      <type>dlib::array&lt; dlib::point &gt;</type>
      <name>centers</name>
      <anchorfile>structgaze_1_1util_1_1_data.html</anchorfile>
      <anchor>abd204ff62fe351c3f1a5bce1c80ee477</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::map&lt; std::string, double &gt;</type>
      <name>execution_times</name>
      <anchorfile>structgaze_1_1util_1_1_data.html</anchorfile>
      <anchor>ac56ed1071903482d1d9b88dd7538cbd4</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>cv::Mat</type>
      <name>source_image</name>
      <anchorfile>structgaze_1_1util_1_1_data.html</anchorfile>
      <anchor>a8406634aa145a854aa389f6bc82c6cd0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>dlib::array2d&lt; dlib::bgr_pixel &gt;</type>
      <name>image</name>
      <anchorfile>structgaze_1_1util_1_1_data.html</anchorfile>
      <anchor>ae36af4e078234ba6bde8a296b6d27746</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>dlib::full_object_detection</type>
      <name>landmarks</name>
      <anchorfile>structgaze_1_1util_1_1_data.html</anchorfile>
      <anchor>a835ed40d954fb1204babf2f8bf8c8b5f</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>dlib::array&lt; dlib::array2d&lt; double &gt; &gt;</type>
      <name>eyes</name>
      <anchorfile>structgaze_1_1util_1_1_data.html</anchorfile>
      <anchor>af550eda81476b72390a0b3da9c25ef94</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>cv::Mat</type>
      <name>head_rotation</name>
      <anchorfile>structgaze_1_1util_1_1_data.html</anchorfile>
      <anchor>a2512c11ebf59eda33a14a482ad36ced6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>cv::Mat</type>
      <name>head_translation</name>
      <anchorfile>structgaze_1_1util_1_1_data.html</anchorfile>
      <anchor>a98d2d33a264de2b3f423ac4e5a5abe0e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; cv::Vec3d &gt;</type>
      <name>pupils</name>
      <anchorfile>structgaze_1_1util_1_1_data.html</anchorfile>
      <anchor>a15f3d6d3022b9521148a5ec74a5dd6a5</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>std::vector&lt; cv::Vec3d &gt;</type>
      <name>gaze_points</name>
      <anchorfile>structgaze_1_1util_1_1_data.html</anchorfile>
      <anchor>a1c53807aa254ecc3be1037a64995c0c8</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>cv::Vec2d</type>
      <name>estimated_gaze_point</name>
      <anchorfile>structgaze_1_1util_1_1_data.html</anchorfile>
      <anchor>a6db092f4cc8c809d6f6c259e8df2364b</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>gui</name>
    <filename>namespacegui.html</filename>
  </compound>
  <compound kind="namespace">
    <name>traits</name>
    <filename>namespacetraits.html</filename>
  </compound>
  <compound kind="namespace">
    <name>wpl</name>
    <filename>namespacewpl.html</filename>
    <class kind="class">wpl::Config</class>
    <class kind="class">wpl::Experiment</class>
  </compound>
  <compound kind="class">
    <name>wpl::Config</name>
    <filename>classwpl_1_1_config.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>assistant_data_complete</name>
      <anchorfile>classwpl_1_1_config.html</anchorfile>
      <anchor>a3595f2140a399337cc05efe7c6d203d1</anchor>
      <arglist>() const </arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>wpl::Experiment</name>
    <filename>classwpl_1_1_experiment.html</filename>
    <member kind="function">
      <type></type>
      <name>Experiment</name>
      <anchorfile>classwpl_1_1_experiment.html</anchorfile>
      <anchor>ad42b3de84ec5f5f75fb27067d8afbe76</anchor>
      <arglist>(GtkImage *const image, Config *const config)</arglist>
    </member>
    <member kind="function">
      <type>Config *</type>
      <name>get_config</name>
      <anchorfile>classwpl_1_1_experiment.html</anchorfile>
      <anchor>ae95f2d77581efa9e370a0176ecb4dfa7</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>prepare</name>
      <anchorfile>classwpl_1_1_experiment.html</anchorfile>
      <anchor>aa385dc8fae9ca894e40b944ef0e26add</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>start</name>
      <anchorfile>classwpl_1_1_experiment.html</anchorfile>
      <anchor>a74ea63fcfaa7b47d733d7d288ec9044c</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>trial</name>
      <anchorfile>classwpl_1_1_experiment.html</anchorfile>
      <anchor>aa73b147dbea4956ddb30a22197003cea</anchor>
      <arglist>()</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static gboolean</type>
      <name>experiment_calibrate</name>
      <anchorfile>classwpl_1_1_experiment.html</anchorfile>
      <anchor>adb96ff11dd51d0e4b57815ba773b1e78</anchor>
      <arglist>(gpointer experiment)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static gboolean</type>
      <name>experiment_trial</name>
      <anchorfile>classwpl_1_1_experiment.html</anchorfile>
      <anchor>a27f1bdc29d1e0c1e2ce4e9938ba6f81d</anchor>
      <arglist>(gpointer experiment)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static gboolean</type>
      <name>experiment_quit_program</name>
      <anchorfile>classwpl_1_1_experiment.html</anchorfile>
      <anchor>a5a0ac004a1d6947845a54432b10bbf83</anchor>
      <arglist>(gpointer experiment)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>experiment_prepare</name>
      <anchorfile>classwpl_1_1_experiment.html</anchorfile>
      <anchor>a78ef70046f9de832fc0fd5c72e99f1ed</anchor>
      <arglist>(const GtkWidget *const assistant, Experiment *const experiment)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>experiment_start</name>
      <anchorfile>classwpl_1_1_experiment.html</anchorfile>
      <anchor>a4e139265922fc76b8d7e9f7677973da0</anchor>
      <arglist>(const GtkWidget *const window, const GdkEventKey *const event_key, Experiment *const experiment)</arglist>
    </member>
  </compound>
  <compound kind="namespace">
    <name>YAML</name>
    <filename>namespace_y_a_m_l.html</filename>
    <class kind="struct">YAML::convert&lt; cv::Point3d &gt;</class>
    <class kind="struct">YAML::convert&lt; cv::Vec3d &gt;</class>
    <class kind="struct">YAML::convert&lt; cv::Mat &gt;</class>
  </compound>
  <compound kind="struct">
    <name>YAML::convert&lt; cv::Point3d &gt;</name>
    <filename>struct_y_a_m_l_1_1convert_3_01cv_1_1_point3d_01_4.html</filename>
    <member kind="function" static="yes">
      <type>static Node</type>
      <name>encode</name>
      <anchorfile>struct_y_a_m_l_1_1convert_3_01cv_1_1_point3d_01_4.html</anchorfile>
      <anchor>a951c97c7d646d2cb02ba9a9acf0e7469</anchor>
      <arglist>(const cv::Point3d &amp;rhs)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>decode</name>
      <anchorfile>struct_y_a_m_l_1_1convert_3_01cv_1_1_point3d_01_4.html</anchorfile>
      <anchor>a0334befda7cc8d9287a94570efaaa4ce</anchor>
      <arglist>(const Node &amp;node, cv::Point3d &amp;rhs)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>YAML::convert&lt; cv::Vec3d &gt;</name>
    <filename>struct_y_a_m_l_1_1convert_3_01cv_1_1_vec3d_01_4.html</filename>
    <member kind="function" static="yes">
      <type>static Node</type>
      <name>encode</name>
      <anchorfile>struct_y_a_m_l_1_1convert_3_01cv_1_1_vec3d_01_4.html</anchorfile>
      <anchor>ab16b28a2e7a10c63fe0625be774bd0f2</anchor>
      <arglist>(const cv::Vec3d &amp;rhs)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>decode</name>
      <anchorfile>struct_y_a_m_l_1_1convert_3_01cv_1_1_vec3d_01_4.html</anchorfile>
      <anchor>a8c8fe9b923c3615dd8b12ceb49bcad03</anchor>
      <arglist>(const Node &amp;node, cv::Vec3d &amp;rhs)</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>YAML::convert&lt; cv::Mat &gt;</name>
    <filename>struct_y_a_m_l_1_1convert_3_01cv_1_1_mat_01_4.html</filename>
    <member kind="function" static="yes">
      <type>static Node</type>
      <name>encode</name>
      <anchorfile>struct_y_a_m_l_1_1convert_3_01cv_1_1_mat_01_4.html</anchorfile>
      <anchor>aad620b02594634c56ebf51710c006f77</anchor>
      <arglist>(const cv::Mat &amp;rhs)</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static bool</type>
      <name>decode</name>
      <anchorfile>struct_y_a_m_l_1_1convert_3_01cv_1_1_mat_01_4.html</anchorfile>
      <anchor>aec0f6ba0ce69ee8f1867c536e5c74649</anchor>
      <arglist>(const Node &amp;node, cv::Mat &amp;rhs)</arglist>
    </member>
  </compound>
  <compound kind="dir">
    <name>src/demo</name>
    <path>/home/runner/gaze/src/demo/</path>
    <filename>dir_ea2db1c5c9ef405bfa9ccf9d4f68ec9b.html</filename>
    <dir>src/demo/simple_tracking</dir>
    <dir>src/demo/where_people_look</dir>
  </compound>
  <compound kind="dir">
    <name>src/gaze</name>
    <path>/home/runner/gaze/src/gaze/</path>
    <filename>dir_b6f340b410c84040e1c3e0fbf05f52e5.html</filename>
    <dir>src/gaze/gui</dir>
    <dir>src/gaze/pipeline_steps</dir>
    <dir>src/gaze/util</dir>
    <file>gaze_tracker.cpp</file>
    <file>pipeline.cpp</file>
    <file>pipeline_step.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/gaze</name>
    <path>/home/runner/gaze/include/gaze/</path>
    <filename>dir_fc1ab1e3ef5cc41f53ed73a231ff7c72.html</filename>
    <dir>include/gaze/gui</dir>
    <dir>include/gaze/pipeline_steps</dir>
    <dir>include/gaze/tests</dir>
    <dir>include/gaze/util</dir>
    <file>gaze.h</file>
    <file>gaze_tracker.h</file>
    <file>pipeline.h</file>
    <file>pipeline_step.h</file>
    <file>pipeline_steps.h</file>
    <file>util.h</file>
  </compound>
  <compound kind="dir">
    <name>src/gaze/gui</name>
    <path>/home/runner/gaze/src/gaze/gui/</path>
    <filename>dir_a47d8e5b193aad337a8154707b5bf79f.html</filename>
    <file>debug_window.cpp</file>
    <file>event_manager.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/gaze/gui</name>
    <path>/home/runner/gaze/include/gaze/gui/</path>
    <filename>dir_1898f472cc575123facc85d17aa2361e.html</filename>
    <file>debug_window.h</file>
    <file>event_manager.h</file>
    <file>visualizeable.h</file>
  </compound>
  <compound kind="dir">
    <name>include</name>
    <path>/home/runner/gaze/include/</path>
    <filename>dir_d44c64559bbebec7f509842c48db8b23.html</filename>
    <dir>include/gaze</dir>
    <dir>include/where_people_look</dir>
  </compound>
  <compound kind="dir">
    <name>src/gaze/pipeline_steps</name>
    <path>/home/runner/gaze/src/gaze/pipeline_steps/</path>
    <filename>dir_25d03f84b58717113c934acd3bb7d176.html</filename>
    <file>eye_like.cpp</file>
    <file>face_landmarks.cpp</file>
    <file>fallback_step.cpp</file>
    <file>gaze_point_calculation.cpp</file>
    <file>head_pose_estimation.cpp</file>
    <file>pupil_localization.cpp</file>
    <file>source_capture.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/gaze/pipeline_steps</name>
    <path>/home/runner/gaze/include/gaze/pipeline_steps/</path>
    <filename>dir_e684859f546307306d24916d644b00b6.html</filename>
    <file>eye_like.h</file>
    <file>face_landmarks.h</file>
    <file>fallback_step.h</file>
    <file>gaze_point_calculation.h</file>
    <file>head_pose_estimation.h</file>
    <file>pupil_localization.h</file>
    <file>source_capture.h</file>
  </compound>
  <compound kind="dir">
    <name>src/demo/simple_tracking</name>
    <path>/home/runner/gaze/src/demo/simple_tracking/</path>
    <filename>dir_82952f9a0c6d7b13fd3cae3720864b25.html</filename>
    <file>simple_tracking.cpp</file>
  </compound>
  <compound kind="dir">
    <name>src</name>
    <path>/home/runner/gaze/src/</path>
    <filename>dir_68267d1309a1af8e8297ef4c3efbcdba.html</filename>
    <dir>src/demo</dir>
    <dir>src/gaze</dir>
  </compound>
  <compound kind="dir">
    <name>include/gaze/tests</name>
    <path>/home/runner/gaze/include/gaze/tests/</path>
    <filename>dir_899b88c3877b96312df4fc8c67d2b265.html</filename>
    <file>doctest_helper.h</file>
  </compound>
  <compound kind="dir">
    <name>src/gaze/util</name>
    <path>/home/runner/gaze/src/gaze/util/</path>
    <filename>dir_ba10b328be7e6cfaa4bdc766f08c5a30.html</filename>
    <file>config.in.cpp</file>
    <file>data.cpp</file>
    <file>dlibcv.cpp</file>
    <file>pipeline_utils.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/gaze/util</name>
    <path>/home/runner/gaze/include/gaze/util/</path>
    <filename>dir_c46cb3223559d6c83e592d78a3316fa8.html</filename>
    <file>config.h</file>
    <file>data.h</file>
    <file>dlibcv.h</file>
    <file>pipeline_utils.h</file>
  </compound>
  <compound kind="dir">
    <name>src/demo/where_people_look</name>
    <path>/home/runner/gaze/src/demo/where_people_look/</path>
    <filename>dir_8eef2932a6597de779b657864daaacc5.html</filename>
    <file>config.cpp</file>
    <file>experiment.cpp</file>
    <file>gui_cb.cpp</file>
    <file>where_people_look.cpp</file>
  </compound>
  <compound kind="dir">
    <name>include/where_people_look</name>
    <path>/home/runner/gaze/include/where_people_look/</path>
    <filename>dir_ee1044f94e919047e58e74dd18f60018.html</filename>
    <file>config.h</file>
    <file>experiment.h</file>
    <file>gui_cb.h</file>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title></title>
    <filename>index</filename>
  </compound>
</tagfile>
