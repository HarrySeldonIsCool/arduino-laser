mod arduino;
use self::arduino::*;
use gtk::prelude::*;
use gtk::{Application, ApplicationWindow, Button};

fn main() -> Result<(), std::io::Error>{
    let app = Application::builder()
        .application_id("org.gtk-rs.example")
        .build();

    app.connect_activate(build_ui);
    // Run the application
    app.run();
    
    //let r = get_data()?;
    Ok(())
}

fn build_ui(app: &Application){
    let window = ApplicationWindow::builder()
        .application(app)
        .title("My GTK App")
        .build();
        
    let button = Button::builder()
        .label("Press me!")
        .margin_top(12)
        .margin_bottom(12)
        .margin_start(12)
        .margin_end(12)
        .build();

    button.connect_clicked(move |button| {button.set_label("Hello world!")});

    window.set_child(Some(&button));
    // Present window to the user
    window.present();
}